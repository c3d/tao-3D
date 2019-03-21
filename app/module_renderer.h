#ifndef TAO_MODULE_RENDERER_H
#define TAO_MODULE_RENDERER_H
// ****************************************************************************
//  module_renderer.h                                              Tao project
// ****************************************************************************
//
//   File Description:
//
//    Enables a module to render OpenGL graphics in a Tao document
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
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao/module_api.h"
#include "drawing.h"

namespace Tao
{

struct Widget;

struct ModuleRenderer : Drawing
// ------------------------------------------------------------------------
//   A special kind of drawing that invokes a callback to do the rendering
// ------------------------------------------------------------------------
{
    ModuleRenderer(ModuleApi::render_fn callback, void * arg)
        : Drawing(), callback(callback), identify(NULL), arg(arg), del(NULL) {}
    ModuleRenderer(ModuleApi::render_fn callback, void * arg,
                   ModuleApi::delete_fn del)
        : Drawing(), callback(callback), identify(NULL), arg(arg), del(del) {}
    ModuleRenderer(ModuleApi::render_fn callback,
                   ModuleApi::render_fn identify, void * arg,
                   ModuleApi::delete_fn del)
        : Drawing(), callback(callback), identify(identify), arg(arg), del(del){}

    virtual ~ModuleRenderer();

    // Drawing interface
    virtual void  Draw(Layout *where);
    virtual void  Identify(Layout *where);

    // Exported to ModuleApi
    static bool   ScheduleRender(ModuleApi::render_fn callback, void *arg);
    static bool   AddToLayout(ModuleApi::render_fn callback, void *arg,
                              ModuleApi::delete_fn del);
    static bool   AddToLayout2(ModuleApi::render_fn callback,
                               ModuleApi::render_fn identify,
                               void *arg,
                               ModuleApi::delete_fn del);


    // Set drawing attributes
    static bool     SetTextures();
    static bool     SetShader(int id);

    static bool     SetFillColor();
    static bool     SetLineColor();

    static Matrix4  ModelMatrix();

private:    
    ModuleApi::render_fn   callback, identify;
    void *                 arg;
    ModuleApi::delete_fn   del;
    static Layout* currentLayout;
};

}

#endif // TAO_MODULE_RENDERER_H
