#ifndef UPDATE_APPLICATION_H
#define UPDATE_APPLICATION_H
// ****************************************************************************
//  update_application.h                                            Tao project
// ****************************************************************************
//
//   File Description:
//
//     Define how update Tao application.
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
//  (C) 2012 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2012 Taodyne SAS
// ****************************************************************************

#include "http_ua.h"
#include "repository.h"
#include "tao_process.h"
#include "tao.h"
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QProgressDialog>
#include <QFile>
#include <QtNetwork>
#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QProgressBar>

namespace Tao {


struct UpdateApplication : QObject, HttpUserAgent
// ------------------------------------------------------------------------
//   Asynchronously update the main application
// ------------------------------------------------------------------------
{
private:
    Q_OBJECT

    enum State { Idle, WaitingForUpdate, Downloading, Downloaded,
                 NetworkErrorCheck, NetworkErrorDownload };

public:
    UpdateApplication();
    ~UpdateApplication();

    void      check(bool show = false);
    QDateTime lastChecked();

private:
    void     startDownload();
    void     readIniFile();
    bool     createFile();
    void     saveDownloadedData();
    void     showNoUpdateAvailable();
    void     showDownloadSuccessful();
    void     resetRequest();
    QString  appName();
    QString  remoteVer();
    void     connectSignals(QNetworkReply *reply);
    void     setLastChecked(QDateTime when);

public slots:
    void     cancel();

private slots:
    void     processReply();
    void     downloadFinished();
    void     downloadProgress(qint64 bytesRcvd, qint64 bytesTotal);
    void     networkError(QNetworkReply::NetworkError err);

    std::ostream & debug();

private:
    State                    state;

    // Update info
    double                   remoteVersion;
    QUrl                     url;
    QString                  description;

    // I/O
    QFile *                  file;
    QProgressDialog *        progress;
    bool                     show;
    QString                  dialogTitle;
    QPixmap                  downloadIcon,
                             checkmarkIcon,
                             connectionErrorIcon;

    // Network
    QNetworkReply *          reply;
    QNetworkRequest          request;
    QNetworkAccessManager *  manager;
    QTime                    downloadTime;
    int                      code;
    QString                  errorString;
};

}

#endif // UPDATE_APPLICATION_H
