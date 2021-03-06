#ifndef HTTP_UA_H
#define HTTP_UA_H
// *****************************************************************************
// http_ua.h                                                       Tao3D project
// *****************************************************************************
//
// File description:
//
//     Base class for HTTP user agents.
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2013-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2012-2013, Jérôme Forissier <jerome@taodyne.com>
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
#include <QString>


namespace Tao {


class HttpUserAgent
// ------------------------------------------------------------------------
//   Base class for HTTP user agents
// ------------------------------------------------------------------------
{
public:
    HttpUserAgent();
    ~HttpUserAgent() {}

    QString                  userAgent();

protected:
    // Tao info
    double                   version;
    QString                  edition;
    QString                  target;

};

}

#endif // HTTP_UA_H
