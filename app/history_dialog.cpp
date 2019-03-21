// ****************************************************************************
//  history_dialog.cpp                                             Tao project
// ****************************************************************************
//
//   File Description:
//
//    HistoryDialog implementation.
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

#include "tao.h"
#include "history_dialog.h"
#include "repository.h"
#include "ui_history_dialog.h"
#include "commit_table_widget.h"
#include "tao_utf8.h"
#include <QMessageBox>
#include <QLineEdit>

namespace Tao {

HistoryDialog::HistoryDialog(Repository *repo, QWidget *parent)
// ----------------------------------------------------------------------------
//    Create a "History" dialog box
// ----------------------------------------------------------------------------
    : QDialog(parent), repo(repo)
{
    XL_ASSERT(repo);

    setupUi(this);
    historyFrame->setRepository(repo);
}


QString HistoryDialog::rev()
// ----------------------------------------------------------------------------
//    Return ID of currently selected commit
// ----------------------------------------------------------------------------
{
    return historyFrame->rev();
}

}
