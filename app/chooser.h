#ifndef CHOOSER_H
#define CHOOSER_H
// ****************************************************************************
//  chooser.h                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//    The chooser is a way to select a command or item in a large set.
//    It works by showing a list, and then allowing you to restrict that
//    list by typing keystrokes.
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
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "activity.h"
#include "tao_tree.h"


TAO_BEGIN

struct ChooserItem
// ----------------------------------------------------------------------------
//   An entry in a chooser
// ----------------------------------------------------------------------------
{
    ChooserItem(text c, XL::Tree *f): caption(c), function(f) {}
    text        caption;
    XL::Tree_p  function;
};
typedef std::vector<ChooserItem> ChooserItems;


struct Chooser : Activity
// ----------------------------------------------------------------------------
//  A chooser shows a selection among the possible commands
// ----------------------------------------------------------------------------
{
    Chooser(SourceFile *program, text name, Widget *w);
    virtual ~Chooser();

    // Callbacks, return true if this activity 'handled' the event
    virtual Activity *  Display(void);
    virtual Activity *  Idle(void);
    virtual Activity *  Key(text key);
    virtual Activity *  Click(uint button, uint count, int x, int y);
    virtual Activity *  MouseMove(int x, int y, bool active);

    // Add an element to a chooser
    void                AddItem(text caption, XL::Tree *function);
    void                AddCommands(text prefix, text label);
    void                SetCurrentItem(text caption);

public:
    SourceFile * xlProgram;
    text         keystrokes;
    int          item;
    int          firstItem;
    ChooserItems items;
    XL::Tree_p   selected;
};



// ============================================================================
// 
//   Entering chooser items in the symbols table
// 
// ============================================================================

extern void EnterChooser();
extern void DeleteChooser();

TAO_END

#endif // CHOOSER_H
