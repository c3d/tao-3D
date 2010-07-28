#ifndef GIT_BACKEND_H
#define GIT_BACKEND_H
// ****************************************************************************
//  git_backend.h                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//     Storing Tao documents in a Git repository.
//
//
//
//
//
//
//
// ****************************************************************************
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao.h"
#include "tree.h"
#include "repository.h"
#include "process.h"
#include "ansi_textedit.h"

#include <QString>
#include <QProcess>
#include <QtGlobal>
#include <QTimer>
#include <iostream>
#include <sys/stat.h>

namespace Tao {

class GitRepository : public Repository
// ----------------------------------------------------------------------------
//   A Git repository
// ----------------------------------------------------------------------------
{
    Q_OBJECT

public:
    GitRepository(const QString &path);
    virtual ~GitRepository() {}

public:
    // Redefine Repository virtual functions
    virtual bool        valid();
    virtual bool        initialize();
    virtual text        branch();
    virtual bool        checkout(text branch);
    virtual bool        branch(text name);
    virtual QStringList branches();
    virtual bool        addBranch(QString name, bool force = false);
    virtual bool        delBranch(QString name, bool force = false);
    virtual bool        renBranch(QString oldName, QString newName, bool force = false);
    virtual bool        isRemoteBranch(text branch);
    virtual bool        add(text name);
    virtual bool        change(text name);
    virtual bool        remove(text name);
    virtual bool        rename(text from, text to);
    virtual bool        commit(text message = "", bool all=false);
    virtual bool        revert(text id);
    virtual bool        cherryPick(text id);
    virtual bool        merge(text branch, ConflictResolution how = CR_Manual);
    virtual bool        reset(text commit = "");
    virtual bool        pull();
    virtual bool        push(QString pushUrl);
    virtual bool        fetch(QString url);
    virtual QStringList remotes();
    virtual QString     remoteFetchUrl(QString name);
    virtual QString     remotePushUrl(QString name);
    virtual bool        addRemote(QString name, QString url);
    virtual bool        setRemote(QString name, QString url);
    virtual bool        delRemote(QString name);
    virtual bool        renRemote(QString oldName, QString newName);
    virtual QList<Commit> history(QString branch = "", int max = 20);
    virtual Process *   asyncClone(QString cloneUrl, QString newFolder,
                                   AnsiTextEdit *out = NULL, void *id = NULL);
    virtual text        version();
    virtual bool        isClean();
    virtual QString     url();
    virtual bool        gc();

    static  bool        checkGit();

protected:
    virtual QString     command();
    virtual QString     userVisibleName();
    virtual text        styleSheet();

protected slots:
    virtual void        asyncProcessFinished(int exitCode);
    void                clearCachedDocVersion();
    virtual void        checkCurrentBranch();

private:
    bool                initialCommit();
    bool                parseCommitOutput(text output, QString &id,
                                          QString &msg);
    QString             parseCloneOutput(QString out);
    void                mergeCommitMessages(text &dest, text src);
    QStringList         crArgs(ConflictResolution mode);

    static QString      gitCommand;
    text                nextCommitMessage;
    text                cachedDocVersion;
    QTimer              cdvTimer;
    time_t              currentBranchMtime;
};

}

#endif // GIT_BACKEND_H