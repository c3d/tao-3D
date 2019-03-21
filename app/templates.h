#ifndef TEMPLATES_H
#define TEMPLATES_H
// ****************************************************************************
//  templates.h                                                    Tao project
// ****************************************************************************
//
//   File Description:
//
//    Enumerate, open and copy Tao document templates.
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
//  (C) 2011 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

/*
 Document templates are used by the New Document wizard (File > New Document).
 Basically they are regular Tao documents that are copied into the user's
 document folder to serve as an example/starting point for a new document.

 Like regular Tao documents:
   - The entry point is a .ddd file
   - They may have associated resources (images, fonts, other .xl files...)

 They have the following additional properties:
   - They are stored in the Tao application directory, under templates/. Each
     template is in its own subdirectory.
   - They have a thumbnail picture (to be displayed by the New Document wizard)
   - They have a property file, template.ini, that contains information about
     the template:
         * name: the name displayed by the New Document wizard
         * thumbnail: the name of the picture file to use as a thubmnail
         * main: the name of the main .ddd file of the document. When copied by
           the New Document wizard, this file will be renamed with the name
           the user has choosen for his/her new document.
 */

#include "tao.h"

#include <QList>
#include <QString>
#include <QPixmap>
#include <QDir>

#include <iostream>

TAO_BEGIN

class Template
// ----------------------------------------------------------------------------
//   A document template
// ----------------------------------------------------------------------------
{
public:
    Template(const QDir &dir);

    bool isValid() { return valid; }
    bool copyTo(QDir &dst);
    bool contains(const QString &keyword, bool searchSource = false);
    QString mainFileFullPath();

public:
    static bool recursiveCopy(const QDir &src, QDir &dst,
                              QString mainFile = "");
    bool operator<(const Template o) const;

private:
    static std::ostream& debug();

public:
    QString name;       // Template name. Not empty if isValid().
    QString description; // Template description. Not empty if isValid().
    QString type;       // Template type. May be null. If it is, it's considered as a theme.
    QPixmap thumbnail;  // Thumbnail picture. May be null.
    QString mainFile;   // Path to the main .ddd file. May be empty.
    QString source;     // .ddd content, non-null only after full text search.

private:
    QDir    dir;        // The template's directory
    bool    valid;      // True if template could be read
    QString thumbFile;  // Name of thumbnail file. May be empty or not exist.
};


class Templates : public QList<Template>
// ----------------------------------------------------------------------------
//   A container for document templates
// ----------------------------------------------------------------------------
{
public:
    Templates() {}
    Templates(const QDir &dir);
    Templates(const QList<QDir> &dirs);

private:
    std::ostream& debug();
    void          read(const QDir &dir);

private:
    QDir dir;  // Parent directory
};



TAO_END

#endif // TEMPLATES_H
