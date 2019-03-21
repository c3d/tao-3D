#ifndef GROUP_LAYOUT_H
#define GROUP_LAYOUT_H
// ****************************************************************************
//  group_layout.h                                                 Tao project
// ****************************************************************************
//
//   File Description:
//
//     A logical group of graphical objetcs (Drawings).
//     A GroupLayout typically contains Shapes or other GroupLayouts.
//     Objects in a group can be selected simultaneously by a single click,
//     and navigation through the group hierarchy is possible with double
//     clicks.
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "layout.h"

TAO_BEGIN


struct GroupLayout : Layout
// ----------------------------------------------------------------------------
//   A logical group of Drawings, selectable as a whole or individually
// ----------------------------------------------------------------------------
{
                        GroupLayout(Widget *widget, Tree_p self);
                        GroupLayout(const GroupLayout &o);

    virtual void        DrawSelection(Layout *);
    virtual void        Add (Drawing *d);
    virtual uint        Selected();

protected:
    Tree_p              self;
};

TAO_END

#endif // GROUP_H
