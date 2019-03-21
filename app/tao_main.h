#ifndef TAO_MAIN_H
#define TAO_MAIN_H
// ****************************************************************************
//  tao_main.h                                                      Tao project
// ****************************************************************************
// 
//   File Description:
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
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jérôme Forissier <jerome@taodyne.com>
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

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
