#ifndef INSPECTORDIALOG_H
#define INSPECTORDIALOG_H
// *****************************************************************************
// inspectordialog.h                                               Tao3D project
// *****************************************************************************
//
// File description:
//
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011, Catherine Burvelle <catherine@taodyne.com>
// (C) 2019, Christophe de Dinechin <christophe@dinechin.org>
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

#include <QDialog>

namespace Ui {
    class InspectorDialog;
}

class InspectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InspectorDialog(QWidget *parent = 0);
    ~InspectorDialog();

    void addProperty();
    void addProperties();

private:
    Ui::InspectorDialog *ui;
};

#endif // INSPECTORDIALOG_H
