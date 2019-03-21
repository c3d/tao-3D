#ifndef FETCH_DIALOG_H
#define FETCH_DIALOG_H
// ****************************************************************************
//  fetch_dialog.h                                                 Tao project
// ****************************************************************************
//
//   File Description:
//
//    The class to display the "Fetch" dialog box
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

#include "fetch_push_dialog_base.h"

namespace Tao {

class FetchDialog : public FetchPushDialogBase
{
    Q_OBJECT

public:
    FetchDialog(Repository *repo, QWidget *parent = 0);

public slots:
    virtual void accept();

signals:
    void         fetched();

};

}

#endif // FETCH_DIALOG_H
