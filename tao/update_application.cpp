#include "update_application.h"
#include "tree.h"
#include "tao_tree.h"
#include "main.h"
#include "context.h"
#include "version.h"
#include "application.h"
#include <QDir>
#include <QFileDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QTextStream>

namespace Tao {


UpdateApplication::UpdateApplication() : aborted(false), updating(false), useMessage(false)
// ----------------------------------------------------------------------------
//    Constructor
// ----------------------------------------------------------------------------
{
    // Define remote repository for each OS
#if defined(Q_OS_MACX)
    from = "git://git.taodyne.com/software/macosx/";
#elif defined(Q_OS_WIN)
    from = "git://git.taodyne.com/software/win/";
#else
    from = "git://git.taodyne.com/software/linux32/others/";

    // Check if we are on Debian or Ubuntu distribution to get .deb package
    QString cmd("uname");
    QStringList args;
    args << "-a";
    Process cp(cmd, args);
    text errors, output;
    if(cp.done(&errors, &output))
        if(output.find("Ubuntu") || output.find("Debian"))
            from = "git://git.taodyne.com/software/linux32/debian/";
#endif

#ifdef TAO_EDITION
    // Get current edition
    edition = TAO_EDITION;
    from += edition;
#endif

    // Create a message box to display download progress
    progress = new QMessageBox;
    progress->addButton(tr("Cancel"), QMessageBox::RejectRole);
    progress->setWindowModality(Qt::WindowModal);
}


void UpdateApplication::check(bool msg)
// ----------------------------------------------------------------------------
//    Check for new update
// ----------------------------------------------------------------------------
{
    useMessage = msg;

    // No need to update if already doing it
    if(!updating)
    {
        if(!repo)
        {
            // Use empty repository
            repo = RepositoryFactory::repository("", RepositoryFactory::NoLocalRepo);
            connect(repo.data(),
                    SIGNAL(asyncGetRemoteTagsComplete(QStringList)),
                    this,
                    SLOT(processRemoteTags(QStringList)));
        }

        if(repo && repo->valid())
        {
            // Get the list of remote tags and process it
            proc = repo->asyncGetRemoteTags(from);
            repo->dispatch(proc);
            updating = true;
        }
    }
}


void UpdateApplication::update()
// ----------------------------------------------------------------------------
//    Prepare to launch update
// ----------------------------------------------------------------------------
{
    QString name = QString("Tao Presentations %1 %2").arg(edition).arg(version);

    // Ask for update
    QString title = tr("%1 available").arg(name);
    QString msg = tr("A new version of Tao Presentations is available, would you download it now ?");
    int ret = QMessageBox::question(NULL, title, msg,
                                    QMessageBox::Yes | QMessageBox::No);

    if(ret == QMessageBox::Yes)
    {
        // Choose folder
        QString folder = QFileDialog::getExistingDirectory(NULL,
                             tr("Select destination folder"),
                             Application::defaultProjectFolderPath());

        if(! folder.isEmpty())
        {
            // Set complete filename
            to.setFileName(folder + "/" + name + ".tar");
            info.setFile(to);

            // Start update
            start();
        }
    }
    else
    {
        updating = false;

        // Delete process
        proc.clear();
    }
}


void UpdateApplication::start()
// ----------------------------------------------------------------------------
//    Start update
// ----------------------------------------------------------------------------
{
    aborted = false;

    // Show progress of current download
    progress->setWindowTitle(tr("Downloading..."));
    progress->setText(tr("Downloading %1...").arg(info.completeBaseName()));
    progress->show();

    connect(progress, SIGNAL(buttonClicked(QAbstractButton*)), this,
            SLOT(abortDownload()), Qt::UniqueConnection);

    // Begin download of the new version
    proc = repo->asyncArchive(from, to.fileName());
    connect(proc.data(), SIGNAL(finished(int,QProcess::ExitStatus)), this,
            SLOT(onDownloadFinished(int,QProcess::ExitStatus)), Qt::UniqueConnection);
    connect(proc.data(), SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(onDownloadError(QProcess::ProcessError)), Qt::UniqueConnection);
    repo->dispatch(proc);
}


void UpdateApplication::extract()
// ----------------------------------------------------------------------------
//    Extract update
// ----------------------------------------------------------------------------
{
    // Extract update
    QString cmd("tar");
    QStringList args;
    args << "-xf" << info.fileName();
    Process cp(cmd, args, info.path());
    cp.waitForFinished();

    if(cp.done())
    {
        // Show final message
        QString title = tr("Download finished");
        QString msg = tr("%1 has been downloaded successfully.").arg(info.completeBaseName());
        QMessageBox::information(NULL, title, msg);
    }
    else
    {
        // Show error message
        QString msg = tr("Extraction failed.\nExit code: %1")
                         .arg(cp.err);
        QMessageBox::warning(NULL, tr("Error"), msg);
    }
}


void UpdateApplication::processRemoteTags(QStringList tags)
// ----------------------------------------------------------------------------
//   Process the list of remote tags for Tao application
// ----------------------------------------------------------------------------
{
    if(! tags.isEmpty())
    {
        // Get latest version of the remote
        QString latest = tags[tags.size() - 1];
        version = latest.toDouble();

        // Get current version
        QString ver = GITREV;
        QRegExp rxp("([^-]*)");
        rxp.indexIn(ver);
        double current = rxp.cap(1).toDouble();

        // Update if current version is older than the remote one
        bool upToDate = (current >= version);
        if(!upToDate)
        {
            update();
        }
        else
        {
            // Show an informative message if wanted
            if(useMessage)
            {
                QString title = tr("No update available");
                QString msg = tr("Tao Presentations %1 is up-to-date.").arg(edition);
                QMessageBox::information(NULL, title, msg);
            }

            updating = false;

            // Clear process
            proc.clear();
        }
    }
}


void UpdateApplication::onDownloadFinished(int exitCode, QProcess::ExitStatus status)
// ----------------------------------------------------------------------------
//    Define action on download finished
// ----------------------------------------------------------------------------
{
    // Close progress dialog
    progress->close();

    if (aborted)
        return;

    if (exitCode != QProcess::NormalExit)
        return;    // onDownloadError will handle this case

    bool success = (status ==  QProcess::NormalExit && exitCode == 0);
    if(! success)
    {
        // Show error message
        QString msg = tr("Download failed.\nExit code: %1\n%2")
                         .arg(exitCode).arg(proc->err);
        QMessageBox::warning(NULL, tr("Error"), msg);
    }
    else
    {
        // Extract download
        extract();
    }

    updating = false;
    // Clear process
    proc.clear();
    // Delete file
    to.remove();
}


void UpdateApplication::abortDownload()
// ----------------------------------------------------------------------------
//    Abort current download
// ----------------------------------------------------------------------------
{
    Q_ASSERT(proc);
    Q_ASSERT(repo);

    aborted = true;
    updating = false;

    process_p p = proc;
    proc.clear();
    if (p)
        repo->abort(p);

    to.remove();
}


void UpdateApplication::onDownloadError(QProcess::ProcessError error)
// ----------------------------------------------------------------------------
//    Display error dialog on download error
// ----------------------------------------------------------------------------
{
    if (aborted)
        return;

    updating = false;

    progress->close();
    QString err = Process::processErrorToString(error);
    QString msg = tr("Download failed: %1").arg(err);
    QMessageBox::warning(NULL, tr("Error"), msg);
    to.remove();
}

}
