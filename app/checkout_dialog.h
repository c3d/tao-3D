#ifndef CHECKOUT_DIALOG_H
#define CHECKOUT_DIALOG_H
// ****************************************************************************
//  checkout_dialog.h                                              Tao project
// ****************************************************************************
//
//   File Description:
//
//    The class to display the Checkout dialog box. This is a modeless
//    dialog which enables to checkout any past version of the document into a
//    temporary branch.
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

#include "history_dialog.h"

namespace Tao {

struct Repository;

class CheckoutDialog : public HistoryDialog
{
    Q_OBJECT

public:
    CheckoutDialog(Repository *repo, QWidget *parent = 0);

signals:
    void   checkedOut(QString id);

private slots:
    void   checkout();
};

}

#endif // CHECKOUT_DIALOG_H
