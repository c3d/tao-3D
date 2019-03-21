#ifndef GLYPH_CACHE_H
#define GLYPH_CACHE_H
// ****************************************************************************
//  glyph_cache.h                                                   Tao project
// ****************************************************************************
//
//   File Description:
//
//     Cache transforming glyphs into textures
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
#include "layout.h"
#include "coords.h"
#include "binpack.h"

#include <QFont>
#include <QImage>
#include <QGLContext>
#include <map>

TAO_BEGIN

struct GlyphCache;

struct GlyphCacheEntry
// ----------------------------------------------------------------------------
//   An entry in one of the glyph caches
// ----------------------------------------------------------------------------
{
    Box             bounds;
    Box             texture;
    coord           advance;
    scale           scalingFactor;
    scale           outlineWidth;
    scale           outlineDepth;
    scale           outlineRadius;
    uint            outlineCount;
    uint            outline;
    uint            interior;
    uint            polycount;
};


struct PerFontGlyphCache
// ----------------------------------------------------------------------------
//    Cache storing glyph information for a specific font
// ----------------------------------------------------------------------------
{
    PerFontGlyphCache(const QFont &font);
    ~PerFontGlyphCache();

public:
    typedef GlyphCacheEntry GlyphEntry;

    bool Find(uint code, GlyphEntry &entry);
    bool Find(text word, GlyphEntry &entry);
    void Insert(uint code, const GlyphEntry &entry);
    void Insert(text word, const GlyphEntry &entry);

protected:
    typedef std::map<uint, GlyphEntry>  CodeMap;
    typedef std::map<text, GlyphEntry>  TextMap;

protected:
    friend struct GlyphCache;
    QFont       font;    
    CodeMap     codes;
    TextMap     texts;
    qreal       ascent, descent, leading;
    qreal       baseSize;
};


struct GlyphCache
// ----------------------------------------------------------------------------
//   Cache storing glyph information irrespective of font
// ----------------------------------------------------------------------------
{
    GlyphCache();
    ~GlyphCache();

public:
    typedef     GlyphCacheEntry                 GlyphEntry;
    typedef     PerFontGlyphCache               PerFont;
    typedef     BinPacker::Rect                 Rect;

    void        Clear();
    void        CheckActiveLayout(Layout *where);
    void        RemoveLayout()  { layout = NULL; }

    uint        Width()        { return packer.Width(); }
    uint        Height()       { return packer.Height(); }
    uint        Texture()      { if (dirty) GenerateTexture(); return texture; }

    PerFont *   FindFont(const QFont &font, bool create = false);

    bool        Find(const QFont &font, const uint code, GlyphEntry&,
                     bool create=false, bool interior=false, scale lineWidth=0);
    bool        Find(const QFont &font, text word, GlyphEntry&,
                     bool create=false, bool interior=false, scale lineWidth=0);
    void        Allocate(uint width, uint height, Rect &rect);

    void        GenerateTexture();
    qreal       Ascent(const QFont &font);
    qreal       Descent(const QFont &font);
    qreal       Leading(const QFont &font);
    void        ScaleDown(GlyphEntry &, scale fontScale, scale extra);


protected:
    // We have a special key that distinguish fonts visually
    // Two fonts with slightly differing size are considered equivalent
    struct Key
    {
        Key(const QFont &font):
            font(font) {}
        QFont font;

        uint fontSizeOrder(const QFont &font) const
        {
            int ptSize = font.pointSize();
            if (ptSize < 0)
                ptSize = font.pixelSize();
            uint result = ptSize;
            uint mask = 0;
            // Keep only 2 significant bits
            while (ptSize > 16)
            {
                ptSize >>= 1;
                mask = (mask << 1) | 1;
            }
            result |= mask;
            return result;
        }

        template <class T> int order(T x1, T x2) const
        {
            if (x1 < x2)
                return -1;
            if (x2 < x1)
                return 1;
            return 0;
        }

        int compare(const QFont &f1, const QFont &f2) const
        {
            if (int sizeCmp = order(fontSizeOrder(f1), fontSizeOrder(f2)))
                return sizeCmp;
            if (int weight = order(f1.weight(), f2.weight()))
                return weight;
            if (int style = order(f1.style(), f2.style()))
                return style;
            if (int stretch = order(f1.stretch(), f2.stretch()))
                return stretch;
            if (int family = order(f1.family(), f2.family()))
                return family;
            if (int capitalization = order(f1.capitalization(),
                                           f2.capitalization()))
                return capitalization;
            if (int line = order(f1.overline(), f2.overline()))
                return line;
            if (int line = order(f1.underline(), f2.underline()))
                return line;
            if (int line = order(f1.strikeOut(), f2.strikeOut()))
                return line;
            return 0;
        }

        int compare(uint64 u1, uint64 u2) const
        {
            if(int unit = order(u1, u2))
                return unit;
            return 0;
        }

        bool operator==(const Key &o) const
        {
            return compare(font, o.font) == 0;
        }
        bool operator<(const Key &o) const
        {
            return compare(font, o.font) < 0;
        }
    };

protected:
    typedef std::map<Key, PerFont *> FontMap;
    FontMap     cache;
    BinPacker   packer;
    uint        texture;
    QImage      image;
    bool        dirty;

public:
    static uint defaultSize;
    scale       minFontSize;
    scale       maxFontSize;
    scale       minFontSizeForAntialiasing;
    scale       fontScaling;
    uint        antiAliasMargin;
    uint64      texUnits;
    PerFont *   lastFont;
    const
    QGLContext *GLcontext;
    Layout *    layout;
};

TAO_END

#endif // GLYPH_CACHE_H
