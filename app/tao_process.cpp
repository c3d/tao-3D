// ****************************************************************************
//  process.cpp                                                     Tao project
// ****************************************************************************
// 
//   File Description:
// 
//     Encapsulate a process with a streambuf we can easily write to
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
//  (C) 1992-2014 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2013 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010-2014 Taodyne SAS
// ****************************************************************************

#include "tao_process.h"
#include "tao_utf8.h"
#include "errors.h"
#include "options.h"

#include <cassert>
#include <QMessageBox>
#include <QApplication>
#include <QRegExp>
#include <QDir>



namespace Tao {

ulong Process::snum = 0;

Process::Process(QObject *parent, size_t bufSize)
// ----------------------------------------------------------------------------
//   Create a QProcess without starting it yet
// ----------------------------------------------------------------------------
    : QProcess(parent),
      mode(ReadWrite), id(NULL),
      aborted(false), combine(false), errOutMaxSize(100000)
{
    initialize(bufSize);
}


Process::Process(const QString &cmd,
                 const QStringList &args,
                 const QString &wd,
                 bool  startImmediately,
                 size_t bufSize,
                 bool combine)
// ----------------------------------------------------------------------------
//   Create a QProcess
// ----------------------------------------------------------------------------
    : cmd(cmd), args(args), wd(""), mode(ReadWrite),
      id(NULL), aborted(false), combine(combine), errOutMaxSize(100000)
{
    setWd(wd);
    initialize(bufSize);
    if (startImmediately)
        start();
}


Process::~Process()
// ----------------------------------------------------------------------------
//    Make sure we are done with all the data for that process
// ----------------------------------------------------------------------------
{
    if (pbase())
        done();
}


void Process::setWd(const QString &wd)
// ----------------------------------------------------------------------------
//   Set working directory after checking it is valid
// ----------------------------------------------------------------------------
//   If wd is empty or invalid, working directory defaults to parent's
{
    if (wd.isEmpty())
        return;
    if (QDir(wd).isReadable())
        setWorkingDirectory(wd);
    else
        std::cerr << +tr("Warning: cannot set working directory to "
                         "non-existent or not readable path: %1\n").arg(wd);
}


void Process::start()
// ----------------------------------------------------------------------------
//   Start child process with current parameters (command, arguments...)
// ----------------------------------------------------------------------------
{
    setWd(wd);
    setEnvironment();

    IFTRACE(process)
    {
        QString commandLine = "{" + cmd;
        foreach (QString a, args)
            commandLine += "} {" + a;
        commandLine += "}";
        QString dir = workingDirectory();
        if (dir.isEmpty())
            dir = "<not set>";
        std::cerr << "Process " << num << ": " << +commandLine
                  << " (wd " << +dir << ")\n";
    }

    startTime.start();
    QProcess::start(cmd, args, mode);
}


void Process::start(const QString &program, const QStringList &arguments,
                    QProcess::OpenMode openmode)
// ----------------------------------------------------------------------------
//   Start process. Overrides parent method.
// ----------------------------------------------------------------------------
{
    cmd = program;
    args = arguments;
    mode = openmode;
    start();
}


void Process::start(const QString &program, OpenMode openmode)
// ----------------------------------------------------------------------------
//   Start process. Overrides parent method.
// ----------------------------------------------------------------------------
{
    cmd = program;
    mode = openmode;
    start();
}


bool Process::done(text *errors, text *output)
// ----------------------------------------------------------------------------
//    Return true if the process was successful
// ----------------------------------------------------------------------------
{
    bool ok = true;

    // Flush streambuf
    if (pbase())
        if (sync())
            ok = false;
    delete[] pbase();
    setp(NULL, NULL);

    // Close QProcess
    closeWriteChannel();
    if (state() != NotRunning)
        if (!waitForFinished())
            ok = false;

    int rc = exitCode();
    if (rc)
        ok = false;

    if (output)
        *output = +out;
    if (errors)
        *errors = +err;

    return ok;
}


bool Process::failed()
// ----------------------------------------------------------------------------
//   Return true if the process crashed or returned a non-zero value
// ----------------------------------------------------------------------------
{
  return (exitStatus() != QProcess::NormalExit) || exitCode();
}


QString Process::getTail(uint lines)
// ----------------------------------------------------------------------------
//  Return the last N lines in the output
// ----------------------------------------------------------------------------
{
    int current = 0;
    QString tail = out;
    if (lines != ~0U)
    {
        QRegExp lineSep("[\n\r]");
        for (uint l = 0; l < lines; l++)
        {
            current = tail.lastIndexOf(lineSep, current-1);
            if (current == -1)
                break;
        }
        if (current >= 0)
            tail = tail.mid(current);
        out = tail;
    }
    return tail;
}


QProcessEnvironment Process::getProcessEnvironment()
// ----------------------------------------------------------------------------
//   Return the process environment that will be used to start the process
// ----------------------------------------------------------------------------
{
    QProcessEnvironment env = processEnvironment();
    if (env.isEmpty())
        env = QProcessEnvironment::systemEnvironment();
    return env;
}


void Process::initialize(size_t bufSize)
// ----------------------------------------------------------------------------
//   Initialize the process and streambuf
// ----------------------------------------------------------------------------
{
    num = snum++;

    // Allocate data to be used by the streambuf
    char *ptr = new char[bufSize];
    setp(ptr, ptr + bufSize);

    // Set up the stderr/stdout signal mechanism
    connect(this, SIGNAL(readyReadStandardOutput()),
            this, SLOT(readStandardOutput()));
    connect(this, SIGNAL(readyReadStandardError()),
            this, SLOT(readStandardError()));

    // Set up debug traces
    IFTRACE(process)
    {
        connect(this, SIGNAL(finished(int,QProcess::ExitStatus)),
                this, SLOT(debugProcessFinished(int,QProcess::ExitStatus)));
        connect(this, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(debugProcessError(QProcess::ProcessError)));
    }
}


void Process::setEnvironment()
// ----------------------------------------------------------------------------
//   Set environment variables that have an influence on child process
// ----------------------------------------------------------------------------
{
}


int Process::overflow(int c)
// ------------------------------------------------------------------------
//   Override std::streambuf::overflow() to pipe to process
// ------------------------------------------------------------------------
{
    int error = sync();
    if (!error && c != traits_type::eof())
    {
        assert(pbase() != epptr());
        error = sputc(c);
    }
    return error;
}


int Process::sync()
// ------------------------------------------------------------------------
//   Overriding std::streambuf::sync(), write data to process
// ------------------------------------------------------------------------
{
    if (pbase() != pptr())
    {
        QProcess::write(pbase(), pptr() - pbase());
        setp(pbase(), epptr());
    }
    return 0;
}


void Process::readStandardOutput()
// ------------------------------------------------------------------------
//   Append new stdout data to buffer
// ------------------------------------------------------------------------
{
    QByteArray newOut = QProcess::readAllStandardOutput();
    if (errOutMaxSize)
    {
        out.append(QString::fromUtf8(newOut.data()));
        out = out.right(errOutMaxSize);
    }
    emit standardOutputUpdated(newOut);
}


void Process::readStandardError()
// ------------------------------------------------------------------------
//   Append new stderr data to buffer
// ------------------------------------------------------------------------
{
    QByteArray newErr = QProcess::readAllStandardError();
    QString &where = combine ? out : err;
    if (errOutMaxSize)
    {
        where.append(QString::fromUtf8(newErr.data()));
        where = where.right(errOutMaxSize);
    }
    emit standardErrorUpdated(newErr);
}


void Process::debugProcessFinished(int exitCode, QProcess::ExitStatus st)
// ------------------------------------------------------------------------
//   Print debug traces to stderr when process finishes
// ------------------------------------------------------------------------
{
    int elapsed = startTime.elapsed();
    QString type = exitStatusToString(st);
    std::cerr << +tr("Process %1 finished (exit type: %2, exit code: %3,"
                     " elapsed: %4 ms)\n")
                     .arg(num).arg(type).arg((int)exitCode).arg(elapsed);
    if (!out.isEmpty())
        std::cerr << +tr("Process %1 stdout:\n%2Process %3 stdout end\n")
                         .arg(num).arg(out).arg(num);
    if (!err.isEmpty())
        std::cerr << +tr("Process %1 stderr:\n%2Process %3 stderr end\n")
                         .arg(num).arg(err).arg(num);
}


void Process::debugProcessError(QProcess::ProcessError error)
// ------------------------------------------------------------------------
//   Print debug traces to stderr in case of process error
// ------------------------------------------------------------------------
{
    QString type = processErrorToString(error);
    std::cerr << +tr("Process %1 error: %2\n").arg(num)
                                              .arg(type);
    if (!err.isEmpty())
        std::cerr << +tr("  stderr: %1\n").arg(err);
}


QString Process::processErrorToString(QProcess::ProcessError error)
// ------------------------------------------------------------------------
//   Convert error code to text
// ------------------------------------------------------------------------
{
    switch (error)
    {
    case QProcess::FailedToStart:  return tr("Failed to start");
    case QProcess::Crashed:        return tr("Crashed");
    case QProcess::Timedout:       return tr("Timed out");
    case QProcess::WriteError:     return tr("Write error");
    case QProcess::ReadError:      return tr("Read error");
    case QProcess::UnknownError:   return tr("Unknown error");
    default:                       return tr("???");
    }
}


QString Process::exitStatusToString(QProcess::ExitStatus status)
// ------------------------------------------------------------------------
//   Convert error code to text
// ------------------------------------------------------------------------
{
    switch (status)
    {
    case QProcess::NormalExit:  return tr("Normal exit");
    case QProcess::CrashExit:   return tr("Crash exit");
    default:                    return tr("???");
    }
}

}
