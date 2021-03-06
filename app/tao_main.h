#ifndef TAO_MAIN_H
#define TAO_MAIN_H
// *****************************************************************************
// tao_main.h                                                      Tao3D project
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
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2012,2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2010-2011,2013, Jérôme Forissier <jerome@taodyne.com>
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

#include "tao.h"
#include "main.h"
#include <signal.h>

TAO_BEGIN

struct Main : public XL::Main
// ----------------------------------------------------------------------------
//   Customization of the Main class for Tao
// ----------------------------------------------------------------------------
{
    Main(int argc, char **argv, text name = "xl_tao",
         text syntax = "xl.syntax",
         text style = "xl.stylesheet",
         text builtins = "builtins.xl")
         : XL::Main(argc, argv, name, syntax, style, builtins)
    {
        MAIN = this;
    }

    virtual int  LoadFile(text file, bool updateContext = false,
                          XL::Context *importContext=0,
                          XL::Symbols *importSymbols=0);
    virtual text SearchFile(text input);
    virtual bool Refresh(double delay);
    virtual text Decrypt(text input);
    virtual Tree *Normalize(Tree *input);

    static Main *MAIN;
};

TAO_END

#ifdef CONFIG_MINGW
typedef void (*sig_t) (int);
#endif

extern void signal_handler(int sig);
extern void install_signal_handler(sig_t);
extern void install_first_exception_handler(void);
extern void tao_stack_trace(int fd);

#endif // TAO_MAIN_H
