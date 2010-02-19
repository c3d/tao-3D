#ifndef PAGE_H
#define PAGE_H
// ****************************************************************************
//  page.h                                                          Tao project
// ****************************************************************************
//
//   File Description:
//
//    Off-screen OpenGL rendering to a 2D texture
//
//    A 'PageInfo' associates persistent rendering data to a particular tree
//
//
//
//
//
// ****************************************************************************
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao.h"
#include "tree.h"
#include <map>
#include <QtOpenGL>
#include <QImage>
#include <QTimeLine>
#include <QSvgRenderer>


TAO_BEGIN

struct ImageTextureInfo : XL::Info
// ----------------------------------------------------------------------------
//    Hold information about an image texture
// ----------------------------------------------------------------------------
{
    typedef ImageTextureInfo *          data_t;
    typedef std::map<text, GLuint>      texture_map;
    enum { MAX_TEXTURES = 20 };

    ImageTextureInfo();
    ~ImageTextureInfo();
    void bind(text img);
    operator data_t() { return this; }

    texture_map textures;
};


struct PageInfo : XL::Info
// ----------------------------------------------------------------------------
//    Information about a given page being rendered in a dynamic texture
// ----------------------------------------------------------------------------
//    A PageInfo structure tends to be permanently attached to a tree
//    to record information that we don't want to re-create / destroy
//    at each and every evaluation of the tree
{
    typedef PageInfo *data_t;

    PageInfo(uint width = 32, uint height = 32);
    ~PageInfo();

    void resize(uint width, uint height);
    void begin();
    void end();
    void bind();

    QGLFramebufferObject *render_fbo;
    QGLFramebufferObject *texture_fbo;
};


struct SvgRendererInfo : PageInfo
// ----------------------------------------------------------------------------
//    Hold information about the SVG renderer for a tree
// ----------------------------------------------------------------------------
{
    typedef SvgRendererInfo *                   data_t;
    typedef std::map<text, QSvgRenderer *>      renderer_map;
    enum { MAX_TEXTURES = 20 };

    SvgRendererInfo(QGLWidget *w, uint width=512, uint height=512);
    ~SvgRendererInfo();
    operator data_t() { return this; }
    void bind(text img);

    QGLWidget *         widget;
    renderer_map        renderers;
};

TAO_END

#endif // PAGE_H
