// ****************************************************************************
//  http_ua.h                                                      Tao project
// ****************************************************************************
//
//   File Description:
//
//     Base class for HTTP user agents.
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
//  (C) 2013 Jerome Forissier <jerome@taodyne.com>
//  (C) 2013 Taodyne SAS
// ****************************************************************************

#include "http_ua.h"
#include <QRegExp>
#include "application.h"
#include "tao_process.h"

namespace Tao {

extern const char * GITREV_;


HttpUserAgent::HttpUserAgent()
// ----------------------------------------------------------------------------
//    Constructor
// ----------------------------------------------------------------------------
{
#ifdef TAO_PLAYER
    edition = "player";
#else
    edition = "unified";
#endif

#if defined(Q_OS_MACX)
   target = "MacOSX";
#elif defined(Q_OS_WIN)
   target = "Windows";
#else
   // Check if we are on Debian or Ubuntu distribution to get .deb package
   QString cmd("uname");
   QStringList args;
   args << "-a";
   Process cp(cmd, args);
   text errors, output;
   if(cp.done(&errors, &output))
   {
       // Check OS name
       if(output.find("Ubuntu") != output.npos ||
          output.find("Debian") != output.npos)
           target = "Linux .deb";
       else
           target = "Linux .tar.bz2";

       // Check architecture
       if(output.find("x86_64") != output.npos)
           target += " x86_64";
       else
           target += " x86";
   }
#endif

   // Get current version of Tao
   QString ver = GITREV_;
   QRegExp rxp("([0-9\\.]+)");
   rxp.indexIn(ver);
   version = rxp.cap(1).toDouble();
}


QString HttpUserAgent::userAgent()
// ----------------------------------------------------------------------------
//    HTTP User-Agent string
// ----------------------------------------------------------------------------
{
    QString ua("Tao3D/%1 (%2; %3; %4)");
    return ua.arg(version).arg(Application::editionStr()).arg(edition)
             .arg(target);
}

}
