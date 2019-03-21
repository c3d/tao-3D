// ****************************************************************************
//  font.cpp                                                        Tao project
// ****************************************************************************
//
//   File Description:
//
//     Parsing a font description
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

#include "font.h"
#include "tao_utf8.h"
#include "runtime.h"
#include "options.h"
#include "errors.h"
#include <iostream>
#include <QFontInfo>
#include <QStringList>
#include <QFontDatabase>

TAO_BEGIN

QMap<QPair<QString, QString>, bool> FontParsingAction::exactMatchCache;


Tree *FontParsingAction::Do (Tree *what)
// ----------------------------------------------------------------------------
//   Default action on trees - Do nothing?
// ----------------------------------------------------------------------------
{
    Ooops("Unexpected font entry $1", what);
    return what;
}


Tree *FontParsingAction::DoInteger(Integer *what)
// ----------------------------------------------------------------------------
//   Integers specify the font size
// ----------------------------------------------------------------------------
{
    font.setPointSizeF(fontSizeAdjust(what->value));
    return what;
}


Tree *FontParsingAction::DoReal(Real *what)
// ----------------------------------------------------------------------------
//   Real numbers specify the font size
// ----------------------------------------------------------------------------
{
    font.setPointSizeF(fontSizeAdjust(what->value));
    return what;
}


Tree *FontParsingAction::DoText(Text *what)
// ----------------------------------------------------------------------------
//   Text specifies a font family (optionally a foundry)
// ----------------------------------------------------------------------------
//   We keep the first family that leads to an exact match
{
    if (!exactMatch)
    {
        text family, style;
        text desc = what->value;
        IFTRACE(fontparsing)
            std::cerr << "Parsing font description: '" <<  desc << "'\n";
        size_t slash = desc.find('/');
        if (slash != std::string::npos)
        {
            // Parse string as "family/style"
            family = desc.substr(0, slash);
            style = desc.substr(slash + 1, std::string::npos);
            int size = font.pointSize();
            IFTRACE(fontparsing)
                std::cerr << "  Requesting font: family='" << family
                          << "' style='" << style << "' size=" << size << "\n";
            font = QFontDatabase().font(+family, +style, size);
        }
        else
        {
            // Font workaround for QTBUG-736 (apparently still not fixed in 4.6)
            // http://bugreports.qt.nokia.com/browse/QTBUG-736
            family = what->value;
            if (family == "Times")
                family = "Times New Roman";
            IFTRACE(fontparsing)
                std::cerr << "  Requesting font: family='" << family << "'\n";
            font.setFamily(+family);
        }
        QPair<QString, QString> key(+family, +style);
        if (!exactMatchCache.contains(key))
        {
            exactMatchCache[key] = font.exactMatch();
            IFTRACE(fontparsing)
                std::cerr << "  Caching exact match flag\n";
        }
        exactMatch = exactMatchCache[key];
        IFTRACE(fontparsing)
        {
            std::cerr << "  Returned font:   family='" << +font.family()
#if QT_VERSION > 0x040800
                      << "' style='" << +font.styleName()
#endif
                      << "' size=" << font.pointSize() << "\n"
                      << "  Exact match: " << exactMatch << "\n";
        }
    }
    return what;
}


Tree *FontParsingAction::DoName(Name *what)
// ----------------------------------------------------------------------------
//   A name specifies one of the font attributes
// ----------------------------------------------------------------------------
{
    text name = what->value;
    if (name == "plain" || name == "default" || name == "normal")
    {
        font.setStyle(QFont::StyleNormal);
        font.setWeight(QFont::Normal);
        font.setStretch(QFont::Unstretched);
        font.setUnderline(false);
        font.setStrikeOut(false);
        font.setOverline(false);
        font.setCapitalization(QFont::MixedCase);
    }
    else if (name == "roman" || name == "no_italic")
    {
        font.setStyle(QFont::StyleNormal);
    }
    else if (name == "italic")
    {
        font.setStyle(QFont::StyleItalic);
    }
    else if (name == "oblique")
    {
        font.setStyle(QFont::StyleOblique);
    }
    else if (name == "light")
    {
        font.setWeight(QFont::Light);
    }
    else if (name == "regular" || name == "no_bold")
    {
        font.setWeight(QFont::Normal);
    }
    else if (name == "demibold")
    {
        font.setWeight(QFont::DemiBold);
    }
    else if (name == "bold")
    {
        font.setWeight(QFont::Bold);
    }
    else if (name == "black")
    {
        font.setWeight(QFont::Black);
    }
    else if (name == "mixed_case" || name == "normal_case")
    {
        font.setCapitalization(QFont::MixedCase);
    }
    else if (name == "uppercase")
    {
        font.setCapitalization(QFont::AllUppercase);
    }
    else if (name == "lowercase")
    {
        font.setCapitalization(QFont::AllLowercase);
    }
    else if (name == "small_caps")
    {
        font.setCapitalization(QFont::SmallCaps);
    }
    else if (name == "capitalized")
    {
        font.setCapitalization(QFont::Capitalize);
    }
    else if (name == "ultra_condensed")
    {
        font.setStretch(QFont::UltraCondensed);
    }
    else if (name == "extra_condensed")
    {
        font.setStretch(QFont::ExtraCondensed);
    }
    else if (name == "condensed")
    {
        font.setStretch(QFont::Condensed);
    }
    else if (name == "semi_condensed")
    {
        font.setStretch(QFont::SemiCondensed);
    }
    else if (name == "unstretched" || name == "no_stretch")
    {
        font.setStretch(QFont::Unstretched);
    }
    else if (name == "semi_expanded")
    {
        font.setStretch(QFont::SemiExpanded);
    }
    else if (name == "expanded")
    {
        font.setStretch(QFont::Expanded);
    }
    else if (name == "extra_expanded")
    {
        font.setStretch(QFont::ExtraExpanded);
    }
    else if (name == "ultra_expanded")
    {
        font.setStretch(QFont::UltraExpanded);
    }
    else if (name == "underline" || name == "underlined")
    {
        font.setUnderline(true);
    }
    else if (name == "overline")
    {
        font.setOverline(true);
    }
    else if (name == "strike_out" || name == "strikeout")
    {
        font.setStrikeOut(true);
    }
    else if (name == "kerning")
    {
        font.setKerning(true);
    }
    else if (name == "no_kerning")
    {
        font.setKerning(false);
    }
    else
    {
        Ooops("Unexpected font keyword $1", what);
        return what;
    }
    return what;
}


bool FontParsingAction::SetAttribute(Name *n, Tree *value)
// ----------------------------------------------------------------------------
//   Set an attribute
// ----------------------------------------------------------------------------
{
    text name = n->value;

    if (name == "size"      ||
        name == "slant"     ||
        name == "weight"    ||
        name == "stretch")
    {
        double amount = 0.0;

        Tree *evaluated = context->Evaluate(value);

        if (Integer *iv = evaluated->AsInteger())
            amount = iv->value;
        else if (Real *rv = evaluated->AsReal())
            amount = rv->value;
        else
            return false;

        if (name == "size")
        {
            font.setPointSizeF(amount);
        }
        else if (name == "slant")
        {
            font.setStyle(QFont::Style(amount));
        }
        else if (name == "weight")
        {
            font.setWeight(QFont::Weight(amount * 100));
        }
        else if (name == "stretch")
        {
            font.setStretch(QFont::Stretch(amount * 100));
        }
        else
        {
            return false;
        }
    }

    return true;
}


Tree *FontParsingAction::DoPrefix(Prefix *what)
// ----------------------------------------------------------------------------
//   Evaluate the prefix
// ----------------------------------------------------------------------------
{
    if (Name *name = what->left->AsName())
        if (SetAttribute(name, what->right))
            return what;

    Tree *value = context->Evaluate(what);
    if (value != what)
        return value->Do(this);
    Ooops("Invalid font attribute $1", what);
    return what;
}


Tree *FontParsingAction::DoPostfix(Postfix *what)
// ----------------------------------------------------------------------------
//   Evaluate the postfix
// ----------------------------------------------------------------------------
{
    Tree *value = context->Evaluate(what);
    if (value != what)
        return value->Do(this);
    Ooops("Invalid font attribute $1", what);
    return what;
}


Tree *FontParsingAction::DoInfix(Infix *what)
// ----------------------------------------------------------------------------
//   Split comma-separated lists, otherwise evaluate sides
// ----------------------------------------------------------------------------
{
    if (what->name == ",")
    {
        what->left->Do(this);
        what->right->Do(this);
        return what;
    }
    else if (what->name == "=" || what->name == ":")
    {
        if (Name *name = what->left->AsName())
            if (SetAttribute(name, what->right))
                return what;
    }

    Tree *value = context->Evaluate(what);
    if (value != what)
        return value->Do(this);
    Ooops("Invalid font attribute $1", what);
    return what;
}


Tree *FontParsingAction::DoBlock(Block *what)
// ----------------------------------------------------------------------------
//   Evaluate the block
// ----------------------------------------------------------------------------
{
    return what->child->Do(this);
}


TAO_END
