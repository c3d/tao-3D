#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H
// ****************************************************************************
//  splash_screen.h                                                 Tao project
// ****************************************************************************
//
//   File Description:
//
//     The Tao splash screen, shown at startup and as an "About" box
//
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include <QWidget>
#include <QSplashScreen>
#include <QTextDocument>

class QLabel;

namespace Tao {


class SplashScreen : public QSplashScreen
// ----------------------------------------------------------------------------
//    The Tao splash screen
// ----------------------------------------------------------------------------
{
    Q_OBJECT

public:
    SplashScreen(Qt::WindowFlags flags = 0);

public slots:
    virtual void showMessage(const QString &message,
                             int alignment = Qt::AlignBottom,
                             const QColor &color = QColor(255, 255, 255));

protected slots:
    void         openUrl(QString url);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void drawContents(QPainter *painter);
    virtual bool event(QEvent *e);

    void         showCredits();
    void         showChangelog();

    QString      message;
    QLabel *     label;
    QLabel *     edition;
    bool         mbPressed, urlClicked;
    QTextDocument licencedTo;

signals:
    void dismissed();
};

}

#endif // SPLASH_SCREEN_H
