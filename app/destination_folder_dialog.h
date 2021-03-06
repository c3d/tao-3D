#ifndef DESTINATION_FOLDER_DIALOG_H
#define DESTINATION_FOLDER_DIALOG_H
// *****************************************************************************
// destination_folder_dialog.h                                     Tao3D project
// *****************************************************************************
//
// File description:
//
//    A dialog box for the user to select a destination folder. There is a
//    drop-down list of known locations (paths), and fields to select any other
//    folder. Selected path is returned in native format.
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2013-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011, Jérôme Forissier <jerome@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************

#include <QtGui>
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>

namespace Tao {

class DestinationFolderDialog : public QDialog
{
    Q_OBJECT

public:
    DestinationFolderDialog(QString defaultPath,
                            QStringList &knownLocations,
                            QWidget *parent = 0);

public:
    QString        target();

public slots:
    virtual void   accept();
    virtual void   reject();

protected slots:
    void           comboBoxIndexChanged(int index);
    void           nameChanged();
    void           locationChanged();
    void           chooseLocation();

protected:
    void           updateNameAndLocation(QString path);

protected:
    QStringList    knownLocations;
    QString        saved;
    QComboBox     *combo;
    QLineEdit     *nameLineEdit, *locationLineEdit;
    QPushButton   *locationChooseButton;
    bool           rejected;
};

}

#endif // DESTINATION_FOLDER_DIALOG_H
