#ifndef RESOURCE_MGT_H
#define RESOURCE_MGT_H
// ****************************************************************************
//  resource_mgt.h						   Tao project
// ****************************************************************************
//
//   File Description:
//
//     Manage resource around the main document.
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
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tree_cloning.h"
#include "QFileInfo"

TAO_BEGIN

struct Widget;

struct ResourceMgt : public TreeClone
// ----------------------------------------------------------------------------
//   Action collection resources used by a tree, such as external files
// ----------------------------------------------------------------------------
{
    ResourceMgt(Widget *widget);

    Tree *DoPrefix(Prefix *what);

    void cleanUpRepo();

private :
    Text * getArg(Prefix * what, int pos);
    bool isToBeMoved(QFileInfo &info, QString prefix);
    text integrateFile(QFileInfo info, QString prefix);

    // List of commands using a filename,
    // associated with
    //  -A- the rank of the filename in the argument list. -1 means last one.
    //  -B- the prefix that will determinate the subdir where the file will
    //      be searched and put (if necessary).
    static std::map < text, std::pair < int, text > > cmdFileList;

    // List of QResource prefix and associated directoy where to place files
    static std::map < text, text > subDirList;

    // The list of used files in the program.
    // Useful to cleanup the git repository.
    std::set<text> usedFile;

    // List of already moved files in case of a file is used more than once.
    std::map<text,text> movedFiles;

};


TAO_END

// PREFIX(LoadFile, tree, "load", PARM(file, text), return xl_load(file))
// PREFIX(FillTexture, tree, "texture", PARM(n, text),
//        RTAO(fillTexture(self, n)))
// PREFIX(FillTextureSVG, tree,"svg", PARM(img, text),
//        RTAO(fillTextureFromSVG(self, img)))
// PREFIX(ImageA, tree,"image",
//        PARM(x, real) PARM(y, real) PARM(w, real) PARM(h, real) PARM(img, text),
//        RTAO(image(self, x, y, w, h, img)))
// PREFIX(ImageB, tree,"image",
//        PARM(x, real) PARM(y, real) PARM(img, text),
//        RTAO(image(self, x, y, img)))

// PREFIX(MenuItem, tree, "menu_item",
//        PARM(n, text) PARM(lbl, text) PARM(icon, text) PARM(cable, boolean)
//        PARM(isc, text)  PARM(t, tree),
//        RTAO(menuItem(self, n, lbl, icon, cable, isc, t)))
//
// PREFIX(Menu, tree, "menu", PARM(n, text) PARM(lbl, text) PARM(icon, text),
//        RTAO(menu(self, n, lbl, icon, false)))
//
// PREFIX(SubMenu, tree, "submenu",
//        PARM(name, text) PARM(lbl, text) PARM(icon, text),
//        RTAO(menu(self, name, lbl, icon, true)))


#endif // RESOURCE_MGT_H
