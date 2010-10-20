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
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jérôme Forissier <jerome@taodyne.com>
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao.h"
#include "main.h"

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

    virtual text SearchFile(text input);
    virtual bool Refresh(double delay);

    static Main *MAIN;
};

TAO_END
