#ifndef GIT_BACKEND_H
#define GIT_BACKEND_H
// ****************************************************************************
//  git_backend.h                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//     Storing Tao documents in a Git repository.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao.h"
#include "tree.h"
#include "repository.h"
#include "tao_process.h"
#include "ansi_textedit.h"

#include <QString>
#include <QProcess>
#include <QtGlobal>
#include <QTimer>
#include <iostream>
#include <sys/stat.h>

namespace Tao {

class GitRepository : public Repository
// ----------------------------------------------------------------------------
//   A Git repository
// ----------------------------------------------------------------------------
{
    Q_OBJECT

public:
    GitRepository(const QString &path, bool noLocal = false);
    virtual ~GitRepository() {}

public:
    // Redefine Repository virtual functions
    virtual bool        valid();
    virtual bool        initialize();
    virtual text        branch();
    virtual bool        checkout(text branch);
    virtual bool        branch(text name);
    virtual QStringList branches();
    virtual bool        addBranch(QString name, bool force = false);
    virtual bool        delBranch(QString name, bool force = false);
    virtual bool        renBranch(QString oldName, QString newName, bool force = false);
    virtual bool        isRemoteBranch(text branch);
    virtual bool        add(text name);
    virtual bool        change(text name);
    virtual bool        remove(text name);
    virtual bool        rename(text from, text to);
    virtual bool        commit(text message = "", bool all=false);
    virtual bool        revert(text id);
    virtual bool        cherryPick(text id);
    virtual bool        merge(text branch, ConflictResolution how = CR_Manual);
    virtual bool        reset(text commit = "");
    virtual bool        pull();
    virtual process_p   asyncPush(QString pushUrl);
    virtual bool        fetch(QString url);
    virtual QStringList remotes();
    virtual QString     remoteFetchUrl(QString name);
    virtual QString     remotePushUrl(QString name);
    virtual QString     remoteWithFetchUrl(QString url);
    virtual bool        addRemote(QString name, QString url);
    virtual bool        setRemote(QString name, QString url);
    virtual bool        delRemote(QString name);
    virtual bool        renRemote(QString oldName, QString newName);
    virtual QList<Commit> history(QString branch = "", int max = 20);
    virtual process_p   asyncClone(QString cloneUrl, QString newFolder,
                                   bool shallow = false);
    virtual process_p   asyncFetch(QString what,
                                   bool    forcetags = false);

    virtual process_p   asyncArchive(QString from="", QString to="",
                                     QString format="tar");
    virtual text        version();
    virtual text        versionTag();
    virtual text        head();
    virtual bool        isClean();
    virtual QString     url();
    virtual bool        gc();
    virtual QString     diff(QString a, QString b, bool symetric);
    virtual process_p   asyncGetRemoteTags(QString remote);
    virtual QStringList tags();
    virtual bool        pathIsRoot();

public:
    static  bool        checkGit();
    static  QString     resolveExePath(QString cmd);

protected:
    static  bool        checkGitCmd();
    static  bool        checkCmd(QString cmd, QString var, QString &out);
    static  QString     checkExe(QString cmd);
    static  bool        showGitSelectionDialog();

signals:
    void                percentComplete(int percent);
    void                asyncGetRemoteTagsComplete(QStringList tags);

protected:
    virtual QString     command();
    virtual QString     userVisibleName();
    virtual text        styleSheet();

protected slots:
    void                computePercentComplete();
    virtual void        asyncProcessFinished(int exitCode, QProcess::ExitStatus st);
    void                clearCachedDocVersion();
    virtual void        checkCurrentBranch();

private:
    bool                initialCommit();
    bool                parseCommitOutput(text output, QString &id,
                                          QString &msg);
    QString             parseCloneOutput(QString out);
    void                mergeCommitMessages(text &dest, text src);
    QStringList         crArgs(ConflictResolution mode);
    QStringList         parseLsRemoteTagsOutput(QString output);
    process_p           asyncCloneShallow(QString cloneUrl);

    static QString      gitCommand;
    static QString      gitExecPath, gitTemplateDir;
    static QString      sshAskPassCommand;
#ifdef CONFIG_MINGW
    static QString      detachCommand;
#endif
    text                nextCommitMessage;
    text                cachedDocVersion;
    QTimer              cdvTimer;
    time_t              currentBranchMtime;
    bool                noLocal;

friend class GitProcess;
friend class GitAuthProcess;

};


class GitProcess : public Process
// ----------------------------------------------------------------------------
//   A Git process
// ----------------------------------------------------------------------------
{
public:
    GitProcess(size_t bufSize = 1024)
        : Process(NULL, bufSize), errPos(0), percent(0) {}
    GitProcess(
            const QString &cmd,
            const QStringList &args = QStringList(),
            const QString &workingDirectory = "",
            bool  startImmediately = true,
            size_t bufSize = 1024)
        : Process(cmd, args, workingDirectory, startImmediately, bufSize),
          errPos(0), percent(0) {}
    virtual ~GitProcess() {}

    virtual void setEnvironment();

public:
    int errPos;    // current position in err (% complete parsing)
    int percent;   // % complete
};


// TODO? move to process.h
class GitAuthProcess : public GitProcess
// ----------------------------------------------------------------------------
//   A Git process that may require user authentication (password prompt)
// ----------------------------------------------------------------------------
{
public:
    GitAuthProcess(size_t bufSize = 1024) : GitProcess(bufSize) {};
    GitAuthProcess(
            const QStringList &args = QStringList(),
            const QString &workingDirectory = "",
            bool  startImmediately = true,
            size_t bufSize = 1024)
        : GitProcess("", args, workingDirectory, startImmediately, bufSize) {};
    virtual ~GitAuthProcess() {};

    virtual void setEnvironment();
    virtual void start();
};

}

#endif // GIT_BACKEND_H
