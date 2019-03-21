// ****************************************************************************
//  webui.cpp                                                      Tao project
// ****************************************************************************
//
//   File Description:
//
//    Interface with the web-based document editor.
//
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
//  (C) 2013 Jerome Forissier <jerome@taodyne.com>
//  (C) 2013 Taodyne SAS
// ****************************************************************************

#include "application.h"
#include "base.h"
#include "preferences_pages.h"
#include "tao_process.h"
#include "tao_utf8.h"
#include "webui.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QLatin1String>
#include <QMessageBox>
#include <QRegExp>
#include <QStringList>
#include <QUrl>
#include <QtGlobal>
#include <QUuid>

namespace Tao {

WebUI::WebUI(QObject *parent)
// ----------------------------------------------------------------------------
//   Constructor: make server ready to start
// ----------------------------------------------------------------------------
    : QObject(parent), server(this), port(0), startBrowser(false)
{
    connect(&server, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(serverStartError()));
    connect(&server, SIGNAL(standardOutputUpdated(QByteArray)),
            this,    SLOT(readServerOut(QByteArray)));
    connect(&server, SIGNAL(standardErrorUpdated(QByteArray)),
            this,    SLOT(readServerError(QByteArray)));
    server.setWd(serverDir());
}


WebUI::~WebUI()
// ----------------------------------------------------------------------------
//   Destructor: stop the server
// ----------------------------------------------------------------------------
{
    stopServer();
}


void WebUI::launch(QString path)
// ----------------------------------------------------------------------------
//   Start server (if needed), open editor in default browser
// ----------------------------------------------------------------------------
{
    if (server.state() == QProcess::NotRunning || path != this->path)
    {
        this->path = path;
        startServer();
        // launchBrowser() will be called when server is ready
    }
    else
    {
        launchBrowser();
    }
}


void WebUI::stopServer()
// ----------------------------------------------------------------------------
//   Stop server if it is running
// ----------------------------------------------------------------------------
{
    if (server.state() != QProcess::NotRunning)
    {
        IFTRACE(webui)
            debug() << "Stopping server\n";
        // On Windows, the process can't be stopped with server.terminate()
        // because as stated in the QProcess documentation:
        // "Console applications on Windows that do not run an event loop, or
        // whose event loop does not handle the WM_CLOSE message, can only be
        // terminated by calling kill()."
        // This looks like a clean solution for all platforms.
        server.write("quit\n");
        if (!server.waitForFinished(5000))
            server.kill();
    }
}


void WebUI::startServer(bool andBrowser)
// ----------------------------------------------------------------------------
//   (Re)start server
// ----------------------------------------------------------------------------
{
    startBrowser = andBrowser;
    stopServer();
    QStringList args("server.js");
    if (GeneralPage::webUISecurityTokenEnabled())
    {
        token = QUuid::createUuid().toString().replace("{","").replace("}","");
        args << "-t" << token;
    }
    else
    {
        token = "";
    }
    args << path;
    IFTRACE(webui)
        debug() << "Starting server - args: " << +args.join(",") << "\n";
    server.start(nodePath(), args);
}


void WebUI::serverStartError()
// ----------------------------------------------------------------------------
//   Notify user that server failed to start
// ----------------------------------------------------------------------------
{
    QMessageBox::warning(NULL, tr("Error"),
                         tr("Could not start document editor (server)"));
}


void WebUI::readServerOut(QByteArray newOut)
// ----------------------------------------------------------------------------
//   Launch browser when server says it is listening
// ----------------------------------------------------------------------------
{
    QRegExp re("Server listening on port ([0-9]+)");
    if (startBrowser && QString::fromUtf8(newOut.data()).contains(re))
    {
        port = re.cap(1).toInt();
        launchBrowser();
    }

    IFTRACE(webui)
        std::cout << "[WebUI Server] " << newOut.data() << "\n";
}


void WebUI::readServerError(QByteArray newErr)
// ----------------------------------------------------------------------------
//   Make sure we report errors in WebUI server
// ----------------------------------------------------------------------------
{
    std::cerr << "[WebUI Server] " << newErr.data() << "\n";
}


void WebUI::securitySettingChanged()
// ----------------------------------------------------------------------------
//   If server is running, restart it because user enabled/disabled security
// ----------------------------------------------------------------------------
{
    if (server.state() != QProcess::NotRunning)
        startServer(false);
}


void WebUI::launchBrowser()
// ----------------------------------------------------------------------------
//   Open a web browser window, connected to the Node.js server
// ----------------------------------------------------------------------------
{
    XL_ASSERT(port);
    QString url = QString("http://localhost:%1/?lang=%2").arg(port)
                    .arg(TaoApp->lang);
    if (!token.isEmpty())
        url += "&token=" + token;

    IFTRACE(webui)
        debug() << "Launching browser at: " << +url << "\n";
    if (!QDesktopServices::openUrl(QUrl(url)))
        QMessageBox::warning(NULL, tr("Error"),
                             tr("Could not launch web browser"));
}


std::ostream & WebUI::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[WebUI " << (void*)this << "] ";
    return std::cerr;
}


QString WebUI::nodePath()
// ----------------------------------------------------------------------------
//   The full path to the Node.js executable
// ----------------------------------------------------------------------------
{
    QString path = QCoreApplication::applicationDirPath()
            + QDir::separator() + "nodejs"
            + QDir::separator() + "bin"
            + QDir::separator();
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    path += QLatin1String("node");
#elif defined(Q_OS_WIN)
    path += QLatin1String("node.exe");
#else
#error Please define Node.js path for this platform
#endif
    return path;
}


QString WebUI::serverDir()
// ----------------------------------------------------------------------------
//   The full path to the directory containing server.js
// ----------------------------------------------------------------------------
{
    return QCoreApplication::applicationDirPath()
            + QDir::separator() + "webui"
            + QDir::separator() + "server";
}

} // namespace Tao
