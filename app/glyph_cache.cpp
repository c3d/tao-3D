// ****************************************************************************
//  glyph_cache.cpp                                                 Tao project
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
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jérôme Forissier <jerome@taodyne.com>
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao_gl.h"
#include "tao_glu.h"
#include "glyph_cache.h"
#include "tao_utf8.h"
#include "path3d.h"
#include "save.h"
#include <QFontMetricsF>

TAO_BEGIN

// ============================================================================
//
//   Per-font glyph cache
//
// ============================================================================
//   We have single glyph and word-based variants, because we allow
//   the cache to contain complete rendered words for the complicated
//   cases where we wouldn't know how to layout individual glyphs side by side
//   without the help of Qt. This is known as the "Dosa problem",
//   see http://labs.trolltech.com/blogs/2007/03/30/working-towards-a-unified-
//   text-layout-engine-for-the-free-desktop-software-stack/
//
//   The entries we put and retrieve from the per-font caches are
//   in texel coordinates, so that they don't change when we extend the
//   texture because we run out of space.
//   The entries we return from Find, on the other hand, are OpenGL texture
//   coordinates, i.e. we normalize them in the 0..1 range.

PerFontGlyphCache::PerFontGlyphCache(const QFont &font)
// ----------------------------------------------------------------------------
//   Construct an empty per-font glyph cache
// ----------------------------------------------------------------------------
    : font(font), ascent(0), descent(0), leading(0)
{
    QFontMetricsF fm(font);
    ascent = fm.ascent();
    descent = fm.descent();
    leading = fm.leading();
    baseSize = font.pointSizeF();
}


PerFontGlyphCache::~PerFontGlyphCache()
// ----------------------------------------------------------------------------
//   Clear the per-font glyph cache
// ----------------------------------------------------------------------------
{
    for (CodeMap::iterator ci = codes.begin(); ci != codes.end(); ci++)
    {
        GlyphEntry &e = (*ci).second;
        if (e.interior)
            GL.DeleteLists(e.interior, 1);
        if (e.outline)
            GL.DeleteLists(e.outline, 1);

    }
    for (TextMap::iterator ti = texts.begin(); ti != texts.end(); ti++)
    {
        GlyphEntry &e = (*ti).second;
        if (e.interior)
            GL.DeleteLists(e.interior, 1);
        if (e.outline)
            GL.DeleteLists(e.outline, 1);
    }
}


bool PerFontGlyphCache::Find(uint code, GlyphEntry &entry)
// ----------------------------------------------------------------------------
//   For a given Unicode, find per-font glyph cache entry if it exists
// ----------------------------------------------------------------------------
{
    CodeMap::iterator it = codes.find(code);
    if (it != codes.end())
    {
        entry = (*it).second;
        return true;
    }
    return false;
}


bool PerFontGlyphCache::Find(text word, GlyphEntry &entry)
// ----------------------------------------------------------------------------
//   For a complete UTF-8 word, find per-font glyph cache entry if it exists
// ----------------------------------------------------------------------------
{
    TextMap::iterator it = texts.find(word);
    if (it != texts.end())
    {
        entry = (*it).second;
        return true;
    }
    return false;
}


void PerFontGlyphCache::Insert(uint code, const GlyphEntry &entry)
// ----------------------------------------------------------------------------
//   Insert glyph entry for a single Unicode glyph
// ----------------------------------------------------------------------------
{
    codes[code] = entry;
}


void PerFontGlyphCache::Insert(text word, const GlyphEntry &entry)
// ----------------------------------------------------------------------------
//   Insert glyph entry for a combination of Unicode glyphs
// ----------------------------------------------------------------------------
{
    texts[word] = entry;
}



// ============================================================================
//
//   Unified glyph and word cache
//
// ============================================================================

uint GlyphCache::defaultSize = 128;

GlyphCache::GlyphCache()
// ----------------------------------------------------------------------------
//   Default constructor creates an empty glyph cache with a texture
// ----------------------------------------------------------------------------
    : cache(),
      packer(defaultSize, defaultSize),
      texture(0),
      image(defaultSize, defaultSize, QImage::Format_ARGB32),
      dirty(false),
      minFontSize(30),
      maxFontSize(120),
      minFontSizeForAntialiasing(9),
      fontScaling(2.0),
      antiAliasMargin(3),
      texUnits(1),              // Default is texture unit 0 only
      lastFont(NULL),
      GLcontext(QGLContext::currentContext()),
      layout(NULL)
{
    image.fill(0);
}


GlyphCache::~GlyphCache()
// ----------------------------------------------------------------------------
//   Release the texture we were using
// ----------------------------------------------------------------------------
{
    // Don't delete texture if context has been changed
    // REVISIT glyph cache should support multiple contexts
    if (GLcontext == QGLContext::currentContext())
        if (texture)
            glDeleteTextures(1, &texture);
    Clear();
}


void GlyphCache::Clear()
// ----------------------------------------------------------------------------
//   Clear the glyph cache entirely
// ----------------------------------------------------------------------------
{
    for (FontMap::iterator it = cache.begin(); it != cache.end(); it++)
        delete (*it).second;
    cache.clear();
    packer.Clear();
    image.fill(0);
    lastFont = NULL;
}


void GlyphCache::CheckActiveLayout(Layout *where)
// ----------------------------------------------------------------------------
//   Check if changes in the layout require us to clear the cache
// ----------------------------------------------------------------------------
{
    // Check if there are new texture units active compared to what we had
    if (GL.ActiveTextureUnits() & ~this->texUnits)
    {
        Clear();
        this->texUnits |= GL.ActiveTextureUnits();
    }
    layout = where;
}


GlyphCache::PerFont *GlyphCache::FindFont(const QFont &font, bool create)
// ----------------------------------------------------------------------------
//   Find the per-font information in the cache
// ----------------------------------------------------------------------------
{
    PerFontGlyphCache *perFont = lastFont;
    if (!perFont || perFont->font != font)
    {
        Key key(font);
        FontMap::iterator cacheEntry = cache.find(key);
        if (cacheEntry == cache.end())
        {
            // Create font cache entry if necessary
            if (!create)
                return NULL;
            perFont = new PerFontGlyphCache(font);
            cache[key] = perFont;
        }
        else
        {
            // Get the cache entry we want
            perFont = (*cacheEntry).second;
        }
        lastFont = perFont;
    }
    return perFont;
}


bool GlyphCache::Find(const QFont &font,
                      uint code, GlyphEntry &entry,
                      bool create, bool interior, scale lineWidth)
// ----------------------------------------------------------------------------
//   Find or create a unicode entry in the cache
// ----------------------------------------------------------------------------
{
    PerFont *perFont = FindFont(font, create);
    if (!perFont)
        return false;
    bool found = perFont->Find(code, entry);
    if (!found && !create)
        return false;

    if (!found)
    {
        // Apply a font with scaling
        scale fs = fontScaling;
        uint aam = antiAliasMargin;
        if (font.pointSizeF() < minFontSizeForAntialiasing)
        {
            fs = 1;
            aam = 0;
        }
        QFont scaled(font);
        scaled.setPointSizeF(font.pointSizeF() * fs);
        if (!aam)
        {
            scaled.setStyleStrategy(QFont::NoAntialias);
            aam = 1;
        }

        // We need to create a new entry
        QFontMetricsF fm(scaled);
        QChar qc(code);
        QRectF bounds = fm.boundingRect(qc);
        if (fabs(bounds.x()) > 10 * scaled.pointSizeF() ||
            fabs(bounds.y()) > 10 * scaled.pointSizeF())
        {
            IFTRACE(fonts)
            {
                QString exactMatchText;
                if (!scaled.exactMatch())
                    exactMatchText = "NO ";
                QString msg;
                msg = QString("Warning: font '%1' (%2exact match): character "
                              "'%3' has weird metrics:\n  width %4 height %5 "
                              "x %6 y %7")
                        .arg(scaled.family()).arg(exactMatchText).arg(qc)
                        .arg(bounds.width()).arg(bounds.height())
                        .arg(bounds.x()).arg(bounds.y());
                std::cerr << +msg << "\n";
            }
            // #1161 Workaround font metric bug in TeX Gyre Adventor
            // 'Mountains of Christmas' has the same problem at least
            // on MacOSX 10.6.8
            // All issues have always been with the space character up to now,
            // but handle other characters just in case
            QChar repl;
            if (qc == ' ')
                repl = QChar('l');
            else
                repl = QChar('m');
            bounds = fm.boundingRect(repl);
            IFTRACE(fonts)
            {
                QString msg;
                msg = QString("Using metrics of character '%1' instead:\n"
                              "  width %2 height %3 x %4 y %7")
                        .arg(repl)
                        .arg(bounds.width()).arg(bounds.height())
                        .arg(bounds.x()).arg(bounds.y());
                std::cerr << +msg << "\n";
            }
        }
        scale bx = bounds.x();
        scale by = bounds.y();
        scale bw = bounds.width();
        scale bh = bounds.height();

        bounds = QRectF(bx - fs, by - fs, bw + 2*fs, bh + 2*fs);
        bx = bounds.x();
        by = bounds.y();
        bw = bounds.width();
        bh = bounds.height();
        uint width = ceil(bw);
        uint height = ceil(bh);

        // Allocate a rectangle where we will put the texture (may resize us)
        BinPacker::Rect rect;
        Allocate(width + 2*aam, height + 2*aam, rect);

        // Record glyph information in the entry
        entry.bounds = Box(bx/fs, by/fs, bw/fs, bh/fs);
        entry.texture = Box(rect.x1+aam, rect.y1+aam, width, height);
        entry.advance = fm.width(qc) / fs;
        entry.scalingFactor = fs;
        entry.interior = 0;
        entry.outline = 0;
        entry.outlineWidth = 1.0;
        entry.outlineDepth = 0.0;
        entry.outlineRadius = 0.0;

        // Store the new entry
        perFont->Insert(code, entry);

        // Draw the texture portion for the desired word
        qreal x = rect.x1 + aam - bounds.left();
        qreal y = rect.y2 - aam - bounds.bottom();

        QPainter painter(&image);
        painter.setFont(scaled);
        painter.setBrush(Qt::transparent);
        painter.setPen(Qt::black);
        painter.drawText(QPointF(x+fs, y+fs), QString(qc));

#if 0 // DEBUG CODE
        painter.setPen(QColor(0, 255, 0, 80));
        painter.drawRect(bounds.translated(x, y));

        painter.setPen(QColor(0, 0, 255, 80));
        painter.drawRect(QRectF(x, y, 1, 1));

        painter.setPen(QColor(255, 0, 255, 80));
        painter.drawRect(QRectF(x+bounds.x(), y+bounds.y(), 1, 1));

        painter.setPen(QColor(255, 0, 0, 80));
        painter.drawRect(QRectF(rect.x1, rect.y1,
                                rect.x2-rect.x1, rect.y2-rect.y1));
#endif

        painter.end();

        // We will need to update the texture
        dirty = true;
    }

    // Line width should remain identical even if we scale the font
    lineWidth /= font.pointSizeF() / perFont->baseSize;

    // Check if we want OpenGL display lists:
    // - If the line width is unknown yet
    // - If there is depth and anything impacting it changed
    scale extra = 0;
    if (layout)
    {
        scale depth = layout->extrudeDepth;
        scale radius = layout->extrudeRadius;
        uint  outlineCount = layout->extrudeCount;
        bool  widthChange = entry.outlineWidth != lineWidth;
        bool  depthChange = entry.outlineDepth != depth;
        bool  radiusChange = entry.outlineRadius != radius;
        bool  countChange = entry.outlineCount != outlineCount;
        bool  outlineChange = (widthChange  || depthChange ||
                               radiusChange || countChange);
        if (depth > 0)
            extra = radius;
        if ((!entry.interior && interior) || !entry.outline || outlineChange)
        {
            // Reset font to original size
            QFont scaled(font);
            scaled.setPointSizeF(perFont->baseSize);

            // Draw glyph into a path
            QPainterPath qtPath;
            GraphicPath path;

            qtPath.addText(0, 0, scaled, QString(QChar(code)));
            path.addQtPath(qtPath, -1);

            if (interior)
            {
                // Create an OpenGL display list for the glyph
                if (!entry.interior)
                    entry.interior = GL.GenLists(1);
                entry.polycount = GL.polycount;
                if (layout->extrudeDepth > 0.0)
                {
                    XL::Save<scale> saveDepth (layout->extrudeDepth, -1.0);
                    GL.NewList(entry.interior, GL_COMPILE);
                    path.Draw(layout, Vector3(0,0,0),
                              GL_POLYGON, GLU_TESS_WINDING_POSITIVE);
                    GL.EndList();
                }
                else
                {
                    GL.NewList(entry.interior, GL_COMPILE);
                    path.Draw(layout, Vector3(0,0,0),
                              GL_POLYGON, GLU_TESS_WINDING_POSITIVE);
                    GL.EndList();
                }
                entry.polycount = GL.polycount - entry.polycount;
            }

            if (!entry.outline || outlineChange)
            {
                if (!entry.outline)
                    entry.outline = GL.GenLists(1);

                if (lineWidth > 0)
                {
                    // Render outline in a GL list
                    QPainterPathStroker stroker;
                    stroker.setWidth(lineWidth);
                    stroker.setCapStyle(Qt::FlatCap);
                    stroker.setJoinStyle(Qt::RoundJoin);
                    stroker.setDashPattern(Qt::SolidLine);
                    QPainterPath stroke = stroker.createStroke(qtPath);
                    GraphicPath strokePath;
                    strokePath.addQtPath(stroke, -1);
                    GL.NewList(entry.outline, GL_COMPILE);
                    strokePath.Draw(layout, Vector3(0,0,0),
                                    GL_POLYGON, GLU_TESS_WINDING_POSITIVE);
                    GL.EndList();
                }
                else if (depth > 0.0)
                {
                    // Render outline as a depth border
                    GL.NewList(entry.outline, GL_COMPILE);
                    path.Draw(layout, Vector3(0,0,0), GL_POLYGON, GL_DEPTH);
                    GL.EndList();
                }

                entry.outlineWidth = lineWidth;
                entry.outlineDepth = depth;
                entry.outlineRadius = radius;
                entry.outlineCount = outlineCount;
            }

            // Store the new or updated entry
            perFont->Insert(code, entry);
        }
    }

    // Scale down what we pass back to the caller
    ScaleDown(entry, font.pointSizeF() / perFont->baseSize, extra);
    return true;
}


bool GlyphCache::Find(const QFont &font,
                      text code, GlyphEntry &entry,
                      bool create, bool interior, scale lineWidth)
// ----------------------------------------------------------------------------
//   Find or create a word entry in the cache
// ----------------------------------------------------------------------------
{
    PerFont *perFont = FindFont(font, create);
    if (!perFont)
        return false;
    bool found = perFont->Find(code, entry);
    if (!found && !create)
        return false;

    if (!found)
    {
        // Apply a font with scaling
        scale fs = fontScaling;
        uint aam = antiAliasMargin;
        if (font.pointSizeF() < minFontSizeForAntialiasing)
        {
            fs = 1;
            aam = 0;
        }
        QFont scaled(font);
        scaled.setPointSizeF(font.pointSizeF() * fs);
        if (!aam)
        {
            scaled.setStyleStrategy(QFont::NoAntialias);
            aam = 1;
        }

        // We need to create a new entry
        QFontMetricsF fm(scaled);
        QString qs(+code);
        QRectF bounds = fm.boundingRect(qs);
        scale bx = bounds.x();
        scale by = bounds.y();
        scale bw = bounds.width();
        scale bh = bounds.height();

        bounds = QRectF(bx - fs, by - fs, bw + 2*fs, bh + 2*fs);
        bx = bounds.x();
        by = bounds.y();
        bw = bounds.width();
        bh = bounds.height();
        uint width = ceil(bw);
        uint height = ceil(bh);

        // Allocate a rectangle where we will put the texture (may resize us)
        BinPacker::Rect rect;
        Allocate(width + 2*aam, height + 2*aam, rect);

        // Record glyph information in the entry
        entry.bounds = Box(bx/fs, by/fs, bw/fs, bh/fs);
        entry.texture = Box(rect.x1+aam, rect.y1+aam, width, height);
        entry.advance = fm.width(qs) / fs;
        entry.scalingFactor = fs;
        entry.interior = 0;
        entry.outline = 0;
        entry.outlineWidth = 1.0;
        entry.outlineDepth = 0.0;
        entry.outlineRadius = 0.0;

        // Store the new entry
        perFont->Insert(code, entry);

        // Draw the texture portion for the desired word
        qreal x = rect.x1 + aam - bounds.left();
        qreal y = rect.y2 - aam - bounds.bottom();

        QPainter painter(&image);
        painter.setFont(scaled);
        painter.setBrush(Qt::transparent);
        painter.setPen(Qt::black);
        painter.drawText(QPointF(x+fs, y+fs), qs);
        painter.end();

        // We will need to update the texture
        dirty = true;
    }

    // Line width should remain identical even if we scale the font
    lineWidth /= font.pointSizeF() / perFont->baseSize;

    // Check if we want OpenGL display lists:
    // - If the line width is unknown yet
    // - If there is depth and anything impacting it changed
    scale extra = 0;
    if (layout)
    {
        scale depth = layout->extrudeDepth;
        scale radius = layout->extrudeRadius;
        uint  outlineCount = layout->extrudeCount;
        bool  widthChange = entry.outlineWidth != lineWidth;
        bool  depthChange = entry.outlineDepth != depth;
        bool  radiusChange = entry.outlineRadius != radius;
        bool  countChange = entry.outlineCount != outlineCount;
        bool  outlineChange = (widthChange  || depthChange ||
                               radiusChange || countChange);
        if (depth > 0)
            extra = radius;
        if ((!entry.interior && interior) || !entry.outline || outlineChange)
        {
            // Reset font to original size
            QFont scaled(font);
            scaled.setPointSizeF(perFont->baseSize);

            // Draw glyph into a path
            QPainterPath qtPath;
            GraphicPath path;

            qtPath.addText(0, 0, scaled, QString(+code));
            path.addQtPath(qtPath, -1);

            if (interior)
            {
                // Create an OpenGL display list for the glyph
                if (!entry.interior)
                    entry.interior = GL.GenLists(1);
                entry.polycount = GL.polycount;
                if (layout->extrudeDepth > 0.0)
                {
                    XL::Save<scale> saveDepth (layout->extrudeDepth, -1.0);
                    GL.NewList(entry.interior, GL_COMPILE);
                    path.Draw(layout, Vector3(0,0,0),
                              GL_POLYGON, GLU_TESS_WINDING_POSITIVE);
                    GL.EndList();
                }
                else
                {
                    GL.NewList(entry.interior, GL_COMPILE);
                    path.Draw(layout, Vector3(0,0,0),
                              GL_POLYGON, GLU_TESS_WINDING_POSITIVE);
                    GL.EndList();
                }
                entry.polycount = GL.polycount - entry.polycount;
            }

            if (!entry.outline || outlineChange)
            {
                if (!entry.outline)
                    entry.outline = GL.GenLists(1);

                if (lineWidth > 0)
                {
                    // Render outline in a GL list
                    QPainterPathStroker stroker;
                    stroker.setWidth(lineWidth);
                    stroker.setCapStyle(Qt::FlatCap);
                    stroker.setJoinStyle(Qt::RoundJoin);
                    stroker.setDashPattern(Qt::SolidLine);
                    QPainterPath stroke = stroker.createStroke(qtPath);
                    GraphicPath strokePath;
                    strokePath.addQtPath(stroke, -1);
                    GL.NewList(entry.outline, GL_COMPILE);
                    strokePath.Draw(layout, Vector3(0,0,0),
                                    GL_POLYGON, GLU_TESS_WINDING_POSITIVE);
                    GL.EndList();
                }
                else if (depth > 0.0)
                {
                    // Render outline as a depth border
                    GL.NewList(entry.outline, GL_COMPILE);
                    path.Draw(layout, Vector3(0,0,0), GL_POLYGON, GL_DEPTH);
                    GL.EndList();
                }

                entry.outlineWidth = lineWidth;
                entry.outlineDepth = depth;
                entry.outlineRadius = radius;
                entry.outlineCount = outlineCount;
            }

            // Store the new or updated entry
            perFont->Insert(code, entry);
        }
    }

    // Scale down what we pass back to the caller
    ScaleDown(entry, font.pointSizeF() / perFont->baseSize, extra);
    return true;
}


void GlyphCache::Allocate(uint width, uint height, Rect &rect)
// ----------------------------------------------------------------------------
//   Allocate  a rectangle, resizing it as necessary
// ----------------------------------------------------------------------------
{
    while (!packer.Allocate(width, height, rect))
    {
        uint w = packer.Width();
        uint h = packer.Height();
        do { w <<= 1; } while (w < width);
        do { h <<= 1; } while (h < height);

        // Resize the image and copy in the texture
        image = image.copy(0,0,w,h);
        dirty = true;

        // Resize the rectangle from which we do the texture allocation
        packer.Resize(w, h);
    }
}


void GlyphCache::GenerateTexture()
// ----------------------------------------------------------------------------
//   Copy the current image into our GL texture
// ----------------------------------------------------------------------------
{
    if (!texture)
        GL.GenTextures(1, &texture);

    QImage invert(image);
    invert.invertPixels();
    QImage texImg = QGLWidget::convertToGLFormat(invert).mirrored(false, true);
    GL.BindTexture(GL_TEXTURE_2D, texture);
    GL.TexParameter(GL_TEXTURE_2D,
                     GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    GL.TexParameter(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL.TexParameter(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    GL.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 texImg.width(), texImg.height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texImg.bits());

    dirty = false;
}


qreal GlyphCache::Ascent(const QFont &font)
// ----------------------------------------------------------------------------
//   Return the ascent for the font
// ----------------------------------------------------------------------------
{
    PerFont *pf = FindFont(font, true);
    return pf->ascent * font.pointSizeF() / pf->baseSize;
}


qreal GlyphCache::Descent(const QFont &font)
// ----------------------------------------------------------------------------
//   Return the descent for the font
// ----------------------------------------------------------------------------
{
    PerFont *pf = FindFont(font, true);
    return pf->descent * font.pointSizeF() / pf->baseSize;
}


qreal GlyphCache::Leading(const QFont &font)
// ----------------------------------------------------------------------------
//   Return the leading for the font
// ----------------------------------------------------------------------------
{
    PerFont *pf = FindFont(font, true);
    return pf->leading * font.pointSizeF() / pf->baseSize;
}


void GlyphCache::ScaleDown(GlyphEntry &entry, scale fontScale, scale extra)
// ----------------------------------------------------------------------------
//   Adjust the scale
// ----------------------------------------------------------------------------
{
    // Scale the geometry
    entry.bounds.lower.x *= fontScale;
    entry.bounds.upper.x *= fontScale;
    entry.bounds.lower.y *= fontScale;
    entry.bounds.upper.y *= fontScale;
    entry.advance *= fontScale;
    entry.scalingFactor = fontScale;

    if (extra > 0)
    {
        entry.bounds.lower.x -= extra;
        entry.bounds.lower.y -= extra;
        entry.bounds.upper.x += extra;
        entry.bounds.upper.y += extra;
        entry.advance += 2*extra;
    }
}

TAO_END
