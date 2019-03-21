// ****************************************************************************
//  chooser.cpp                                                     Tao project
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

#include "chooser.h"
#include "widget.h"
#include "context.h"
#include "runtime.h"
#include "utf8.h"
#include "tao_utf8.h"
#include "gl_keepers.h"
#include <wctype.h>
#include <stdio.h>
#include <QFont>
#include <QFontMetricsF>


TAO_BEGIN

using namespace XL;


Chooser::Chooser(SourceFile *program, text name, Widget *w)
// ----------------------------------------------------------------------------
//   Chooser constructor
// ----------------------------------------------------------------------------
    : Activity(name, w), xlProgram(program),
      keystrokes(""), item(0), firstItem(0), selected(NULL)
{}


Chooser::~Chooser()
// ----------------------------------------------------------------------------
//    Chooser destructor
// ----------------------------------------------------------------------------
{}


static bool CharMatch(QChar ref, QChar test)
// ----------------------------------------------------------------------------
//   Character matching function used by incremental search
// ----------------------------------------------------------------------------
//   * Comparison is case-insensitive
//   * Space and underscore are equivalent
//   * Accent folding: a letter without accent matches itself and the same
//     letter with any accent; but an accented letter matches only itself.
//     For instance:   CharMatch('e', 'é') => true
//                     CharMatch('e', 'e') => true
//                     CharMatch('é', 'e') => false
{
    static QMap<QChar, QChar> fold;

    if (fold.empty())
    {
#       define F(from, to) fold[QChar(from)] = QChar(to)
        F(0xE0, 'a'); F(0xE1, 'a'); F(0xE2, 'a'); F(0xE3, 'a'); F(0xE4, 'a');
        F(0xE5, 'a');
        F(0xE8, 'e'); F(0xE9, 'e'); F(0xEA, 'e'); F(0xEB, 'e');
        F(0xEC, 'i'); F(0xED, 'i'); F(0xEE, 'i'); F(0xEF, 'i');
        F(0xF2, 'o'); F(0xF3, 'o'); F(0xF4, 'o'); F(0xF5, 'o'); F(0xF6, 'o');
        F(0xF8, 'o');
        F(0xF9, 'u'); F(0xFA, 'u'); F(0xFB, 'u'); F(0xFC, 'u');
        F(0xFD, 'y'); F(0xFF, 'y');
#       undef F
    }

    if (test.toLower() == ref.toLower())
        return true;
    if ((test.isSpace() || test == '_') &&
        ( ref.isSpace() ||  ref == '_'))
        return true;
    if (fold.contains(test) && fold[test] == ref)
        return true;

    return false;
}


static int KeystrokesFind(QString where, QString what)
// ----------------------------------------------------------------------------
//   Find 'what' in 'where', ignoring capitalization and stuff...
// ----------------------------------------------------------------------------
{
    uint i, maxi = where.length();
    uint j, maxj = what.length();

    for (i = 0; i + maxj <= maxi; i++)
    {
        bool found = true;
        for (j = 0; found && j < maxj; j++)
            found = CharMatch(what[j], where[i + j]);
        if (found)
            return i;
    }
    return -1;
}


Activity *Chooser::Display(void)
// ----------------------------------------------------------------------------
//    Display the chooser
// ----------------------------------------------------------------------------
{
    // Select the chooser font
    XLCall("chooser_title_font") (xlProgram);
    QFont titleFont = widget->currentFont();
    QFontMetricsF titleFM(titleFont);
    coord mtlh = titleFM.height();
    coord mtla = titleFM.ascent();

    // Dimensions data for the chooser
    coord ww = widget->width();
    coord wh = widget->height();
    coord mw = ww / 2;
    coord mh = mtlh * 3 / 2;

    int i, found = 0, displayed = 0;

    // We start with nothing selected
    selected = NULL;

    // Select the chooser item font
    XLCall("chooser_item_font")(xlProgram);
    QFont itemFont = widget->currentFont();
    QFontMetricsF itemFM(itemFont);
    coord milh = itemFM.height();
    coord mild = itemFM.descent();
    coord maxmh = wh - 3 * milh;

    // List the elements in the chooser that match the keystrokes
    ChooserItems remaining;
    ChooserItems::iterator it;
    for (it = items.begin(); it != items.end(); it++)
    {
        text &caption = (*it).caption;
        int pos = KeystrokesFind (+caption, +keystrokes);
        if (pos >= 0)
        {
            // Compute the width of the chooser
            coord mw1 = itemFM.width(+caption);
            if (mw1 > ww)
            {
                text cap = caption;
                size_t keep;
                do
                {
                    keep = cap.length() * ww / mw1;
                    if (keep < 5)
                        keep = 0;
                    else
                        keep -= 5;
                    cap.erase(keep);
                    mw1 = itemFM.width(+cap + "...");
                }
                while ((mw1 > ww) && keep);
                caption = cap + "...";
            }
            if (mw < mw1)
                mw = mw1;

            // Compute the height of the chooser
            if (mh <= maxmh)
            {
                mh += milh;
                displayed++;
            }

            // Add items found to the list
            remaining.push_back(*it);
            found++;
        }
    }
    coord mx = (ww - mw) / 2;
    coord my = (wh - mh) / 2;
    coord ix = mx;
    coord iy = my + mh - mtla;

    // Initialize an "overlay" projection mode
    GLAllStateKeeper saveGL(GL_ALL_ATTRIB_BITS, true, true, 0);
    GL.MatrixMode(GL_PROJECTION);
    GL.LoadIdentity();
    GL.Ortho2D(0, ww, 0, wh);
    GL.MatrixMode(GL_MODELVIEW);
    GL.LoadIdentity();
    GL.LoadMatrix();

    // Draw the chooser box
    widget->drawSelection(NULL, Box3(mx, my, 0, mw, mh, 0), "chooser_box");

    // Show what the user has typed
    if (keystrokes.length())
    {
        XLCall call(found ? "draw_chooser_choice" : "draw_chooser_error");
        call, keystrokes, ix, iy;
        widget->drawCall(NULL, call);
    }
    else
    {
        XLCall call("draw_chooser_title");
        call, name, ix, iy;
        widget->drawCall(NULL, call);
    }
    iy -= mtlh;

    // Adjust the chooser selection
    if (item < 0)
        item = found - 1;
    else if (item >= found)
        item = 0;
    if (displayed == found)
        firstItem = 0;
    if (firstItem > item)
        firstItem = item;
    else if (firstItem + displayed <= item)
        firstItem = item - displayed + 1;

    // List all the chooser items
    for (i = 0; i < displayed; i++)
    {
        text caption = remaining[i + firstItem].caption;

        if (found > displayed)
        {
            // Indicate when there's more above in the list...
            if (i == 0 && firstItem > 0)
                widget->drawSelection(NULL, Box3(ix+mw/2, iy+mild, 0,
                                                 milh, milh, 0),
                                      "chooser_more_above");

            // Indicate when there's more below in the list...
            if (i == displayed-1 && firstItem + i != found - 1)
                widget->drawSelection(NULL, Box3(ix+mw/2, iy+mild, 0,
                                                 milh, milh, 0),
                                      "chooser_more_below");
        }

        // Draw the selection box if we are on selected line
        if (i + firstItem == item)
        {
            widget->drawSelection(NULL,  Box3(mx, iy - mild, 0, mw, milh, 0),
                                  "chooser_selection");
            selected = remaining[i + firstItem].function;
        }

        // Render command selection
        if (keystrokes.length())
        {
            QString qcaption = +caption;
            QString qkeystrokes = +keystrokes;
            int pos = KeystrokesFind(qcaption, qkeystrokes);
            QString ca1 = qcaption.mid(0, pos);
            QString ca2 = qcaption.mid(pos, qkeystrokes.length());
            QString ca3 = qcaption.mid(pos + qkeystrokes.length());
            XLCall call("draw_chooser_match");
            call, +ca1, +ca2, +ca3, ix, iy;
            widget->drawCall(NULL, call);
        }
        else
        {
            XLCall call("draw_chooser_item");
            call, caption, ix, iy;
            widget->drawCall(NULL, call);
        }

        // Move to the next line
        iy -= milh;
    }

    // Display next activity
    return next;
}


Activity *Chooser::Idle(void)
// ----------------------------------------------------------------------------
//   Process remaining idle activities
// ----------------------------------------------------------------------------
{
    return next;
}


Activity *Chooser::Key(text key)
// ----------------------------------------------------------------------------
//    Pressing a key while the chooser is active
// ----------------------------------------------------------------------------
{
    uint position = keystrokes.length();
    Widget *widget = this->widget;
    if (key == "Return" || key == "Enter")
    {
        if (selected)
        {
            xl_evaluate(xlProgram->context, selected);
            delete this;
        }
    }
    else if (key == "Escape")
    {
        delete this;
    }
    else if (key == "Down" || key == "Right")
    {
        item++;
    }
    else if (key == "Up" || key == "Left")
    {
        item--;
    }
    else if (key == "PageUp")
    {
        item -= 8;
    }
    else if (key == "PageDown")
    {
        item += 8;
    }
    else if (key == "Backspace")
    {
        if (position > 0)
        {
            uint prev = Utf8Previous(keystrokes, position);
            keystrokes.erase(prev, position - prev);
        }
    }
    else if (key == "Space")
    {
        keystrokes += " ";
    }
    else if (Utf8Length(key) == 1)
    {
        keystrokes += key;
    }

    // Force an immediate widget refresh
    widget->updateGL();

    // Notify caller that we intercept all keystrokes
    return NULL;
}


Activity *Chooser::Click(uint button, uint count, int x, int y)
// ----------------------------------------------------------------------------
//   When we click in a chooser, it clicks...
// ----------------------------------------------------------------------------
{
    (void) button; (void) count; (void) x; (void) y;
    Activity *n = next;
    delete this;
    return n;
}


Activity *Chooser::MouseMove(int x, int y, bool active)
// ----------------------------------------------------------------------------
//   Mouse movements...
// ----------------------------------------------------------------------------
{
    (void) x; (void) y; (void) active;
    return next;
}


void Chooser::AddItem(text caption, Tree *function)
// ----------------------------------------------------------------------------
//    Add an item to the list
// ----------------------------------------------------------------------------
{
    // Multiple spaces, tabs, newlines... are replaced by a single space
    QString qcap(+caption);
    caption = +qcap.replace(QRegExp("\\s+"), " ");

    items.push_back(ChooserItem(caption, function));
}


void Chooser::AddCommands(text begin, text label)
// ----------------------------------------------------------------------------
//   Add chooser commands from the symbols table
// ----------------------------------------------------------------------------
{
    // Add all the commands that begin with the prefix in the current context
    XL::name_set names, infix, prefix, postfix;
    XL::MAIN->ListNames(begin, names, infix, prefix, postfix);
    
    // Loop over all rewrites that match
    uint first = begin.length();
    for (XL::name_set::iterator n = names.begin(); n != names.end(); n++)
    {
        text symname = *n;
        text caption = "";
        kstring data = symname.data();
        uint c, maxc = symname.length();
        for (c = first; c < maxc; c = Utf8Next(data, c))
        {
            wchar_t wc;
            char wcbuf[MB_LEN_MAX];
            if (mbtowc(&wc, data + c, maxc - c) > 0)
            {
                if (wc == '_')
                    wc = ' ';
                else if (c == first && label.length() > 0)
                    wc = towupper(wc);
            }
            int sz = wctomb(wcbuf, wc);
            if (sz > 0)
                caption.insert(caption.end(), wcbuf, wcbuf + sz);
        }
        
        // Create a closure from the resulting commands to remember context
        Tree *command = new XL::Name(symname);
        caption = widget->xlTr(NULL, caption);
        AddItem(label + caption, command);
    }
}


void Chooser::SetCurrentItem(text caption)
// ----------------------------------------------------------------------------
//   Set current item to be the line that matches 'caption'
// ----------------------------------------------------------------------------
{
    int found = 0;
    ChooserItems::iterator it;
    for (it = items.begin(); it != items.end(); it++, found++)
    {
        if ((*it).caption == caption)
        {
            item = found;
            return;
        }
    }
}

TAO_END



// ****************************************************************************
// 
//    Code generation from chooser.tbl
// 
// ****************************************************************************

#include "graphics.h"
#include "opcodes.h"
#include "options.h"
#include "widget.h"
#include "types.h"
#include "drawing.h"
#include "layout.h"
#include "module_manager.h"
#include <iostream>


// ============================================================================
//
//    Top-level operation
//
// ============================================================================

#include "widget.h"

using namespace XL;

#include "opcodes_declare.h"
#include "chooser.tbl"

namespace Tao
{

#include "chooser.tbl"


void EnterChooser()
// ----------------------------------------------------------------------------
//   Enter all the basic operations defined in attributes.tbl
// ----------------------------------------------------------------------------
{
    XL::Context *context = MAIN->context;
#include "opcodes_define.h"
#include "chooser.tbl"
}


void DeleteChooser()
// ----------------------------------------------------------------------------
//   Delete all the global operations defined in attributes.tbl
// ----------------------------------------------------------------------------
{
#include "opcodes_delete.h"
#include "chooser.tbl"
}

}
