// ****************************************************************************
//  examples_menu.h                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//    Create a menu with several entries to open examples
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
//  (C) 2012 Jerome Forissier <jerome@taodyne.com>
//  (C) 2012 Taodyne SAS
// ****************************************************************************

#include "examples_menu.h"
#include "application.h"
#include "tao_utf8.h"
#include "scanner.h"

#include <sstream>

#include <QAction>
#include <QFileInfo>
#include <QVariant>
#include <QNetworkRequest>


namespace Tao {

ExamplesMenu::ExamplesMenu(QString caption, QWidget *parent) 
// ----------------------------------------------------------------------------
//   Constructor
// ----------------------------------------------------------------------------
    : QMenu(caption, parent), submenus()
{}


ExamplesMenu::~ExamplesMenu()
// ----------------------------------------------------------------------------
//   Destructor
// ----------------------------------------------------------------------------
{
    foreach(ExamplesMenu *submenu, submenus)
        delete submenu;
}


ExamplesMenu *ExamplesMenu::addExample(QString caption,
                                       QString path,
                                       QString tip)
// ----------------------------------------------------------------------------
//   Add an entry to the menu, linking to a particular document
// ----------------------------------------------------------------------------
{
    ExamplesMenu *created = NULL;

    int pathIndex = caption.indexOf('/');
    if (pathIndex >= 0)
    {
        // We have a hierarchical menu
        QString top = caption.left(pathIndex);
        ExamplesMenu *submenu;
        submenus_t::iterator found = submenus.find(top);
        if (found == submenus.end())
        {
            submenu = new ExamplesMenu(top, this);
            submenus[top] = submenu;
            this->addMenu(submenu);
            created = submenu;
        }
        else
        {
            submenu = *found;
        }
        submenu->addExample(caption.mid(pathIndex+1), path, top);
    }
    else
    {
        // If path is relative, prepend the templates directory
        if (!path.contains("://") && path.left(4) != "tao:" &&
            !QFileInfo(path).isAbsolute())
            path = QCoreApplication::applicationDirPath()
                + "/templates/" + path;

        if (caption == "-")
        {
            addSeparator();
        }
        else
        {
            QAction * action = new QAction(caption, this);
            action->setData(QVariant(path));
            if (tip != "")
                action->setToolTip(tip);
            connect(action, SIGNAL(triggered()), this, SLOT(actionTriggered()));
            addAction(action);
        }
    }

    return created;
}


void ExamplesMenu::actionTriggered()
// ----------------------------------------------------------------------------
//   Emit signal with the path of the document to be opened
// ----------------------------------------------------------------------------
{
    QAction * action = dynamic_cast<QAction *>(sender());
    QString path = action->data().toString();
    emit openDocument(path);
}



// ============================================================================
// 
//   Examples menu updates
//
// ============================================================================

ExamplesMenuUpdater::ExamplesMenuUpdater(QUrl url)
// ----------------------------------------------------------------------------
//   Initialize the examples menu updater
// ----------------------------------------------------------------------------
    : QObject(), network()
{
    connect(&network, SIGNAL(finished(QNetworkReply *)),
            this,     SLOT(downloaded(QNetworkReply *)));

    QNetworkRequest req(url);
    network.get(req);
}


void ExamplesMenuUpdater::downloaded(QNetworkReply *reply)
// ----------------------------------------------------------------------------
//   File download is complete - Process the data
// ----------------------------------------------------------------------------
{
    QSettings settings;
    int row = 0;

    settings.beginWriteArray("examples");
    while(!reply->atEnd())
    {
        QString caption = reply->readLine(256);
        QString url = reply->readLine(1024);
        QString description = reply->readLine(1024);
        QString empty = reply->readLine(5);

        caption = caption.trimmed();
        url = url.trimmed();
        description = description.trimmed();
        empty = empty.trimmed();

        IFTRACE(update)
            std::cerr << "Examples menu Entry " << row+1    << "\n"
                      << "    Caption=" << +caption         << "\n"
                      << "    URL    =" << +url             << "\n"
                      << "    Descr  =" << +description     << "\n"
                      << "    Empty  =" << +empty           << "\n";
        
        settings.setArrayIndex(row++);
        settings.setValue("caption", caption);
        settings.setValue("url", url);
        settings.setValue("description", description);

        if (empty != "")
            std::cerr << "Unexpected non-empty line "
                      << row * 4
                      << " in Tao examples list\n";
    }
    settings.endArray();

    // Update menus after the download
    TaoApp->updateHelpMenu();
}


}
