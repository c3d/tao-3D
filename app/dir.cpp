// ****************************************************************************
//  dir.cpp                                                        Tao project
// ****************************************************************************
//
//   File Description:
//
//    Implementation of the Dir class.
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
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "dir.h"
#include "tao_utf8.h"
#include <iostream>

TAO_BEGIN

QFileInfoList Dir::entryInfoGlobList(QString pattern)
// ----------------------------------------------------------------------------
//   Enumerate files that match pattern relative to the current directory
// ----------------------------------------------------------------------------
{
    QFileInfoList result;
    if (pattern.startsWith("/"))
    {
        while (pattern.startsWith("/"))
            pattern.remove(0, 1);
        result << Dir("/").entryInfoGlobList(pattern);
    }
#if defined(Q_OS_WIN)
    // Handle patterns like "E:/some/path"
    else if (pattern.size() >= 3 &&
             pattern[1] == QChar(':') &&
             pattern[2] == QChar('/'))
    {
        int slash = pattern.indexOf("/");
        QString drive = pattern.left(slash + 1); // E:/
        QString rem = pattern.mid(slash + 1);    // some/path
        result << Dir(drive).entryInfoGlobList(rem);
    }
#endif
    else
    {
        int slash = pattern.indexOf("/");
        if (slash != -1)
        {
            QString top = pattern.left(slash);
            QString rem = pattern.mid(slash + 1);
            while (rem.startsWith("/"))
                rem.remove(0, 1);
            QFileInfoList elements = entryInfoList(QStringList() << top,
                                                   QDir::Dirs | QDir::Hidden);
            foreach (QFileInfo elem, elements)
            {
                if (elem.isDir())
                {
                    Dir dir(elem.absoluteFilePath());
                    result << dir.entryInfoGlobList(rem);
                }
            }
        }
        else
        {
            result << entryInfoList(QStringList() << pattern,
                                    QDir::Files | QDir::Hidden);
        }
    }
    return result;
}

TAO_END
