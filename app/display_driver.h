#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H
// ****************************************************************************
//  display_driver.h                                               Tao project
// ****************************************************************************
//
//   File Description:
//
//    Wrapper class for the final rendering function(s) used by a widget to
//    display each frame.
//    This class contains a default implementation which produces a 2D
//    projection, as well as pointers to more sophisticated algorithms
//    typically implemented by modules, for instance to support stereoscopic
//    or multiple views displays.
//    This class also provides entry points for drawing routines in the module
//    API.
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 2011 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************


#include "tao.h"
#include "frame.h"
#include "tao/module_api.h"
#include <QStringList>
#include <QMap>

TAO_BEGIN

struct DisplayDriver
// ----------------------------------------------------------------------------
//   Convert a 3D scene to a format suitable for the display hardware
// ----------------------------------------------------------------------------
{
    DisplayDriver();
    ~DisplayDriver();

    // Methods used by Tao
    void                display();
    bool                setDisplayFunction(QString name);
    QString             getDisplayFunction();
    bool                isCurrentDisplayFunctionSameAs(QString name);
    bool                setOption(std::string name, std::string val);
    std::string         getOption(std::string name, std::string deflt);
    static QStringList  allDisplayFunctions();
    double              windowWidthFactor()  { return wFactor; }
    double              windowHeightFactor() { return hFactor; }
    static int          viewPointsChangedEvent() { return vpEvt; }

public:
    // Methods exported by the module API for use by display modules
    // See module_api.h
    static bool         registerDisplayFunction(std::string name,
                                                ModuleApi::display_fn fn,
                                                ModuleApi::display_use_fn use = NULL,
                                                ModuleApi::display_unuse_fn unuse = NULL,
                                                ModuleApi::display_setopt_fn setopt = NULL,
                                                ModuleApi::display_getopt_fn getopt = NULL);
    static bool         registerDisplayFunctionAlias(std::string name, std::string other);
    static void         drawScene();
    static void         drawSelection();
    static void         drawActivities();
    static void         setGlClearColor();
    static void         setupGl();
    static void         showGlErrors();
    static bool         setStereo(bool on);
    static void         setStereoPlanes(uint planes);
    static void         getCamera(Point3 *pos, Point3 *target, Vector3 *up,
                                  double *toScreen);
    static int          renderHeight();
    static int          renderWidth();
    static double       zNear();
    static double       zFar();
    static double       zoom();
    static double       eyeSeparation();
    static int          getCurrentEye();
    static int          getEyesNumber();
    static void         setProjectionMatrix(int w, int h,
                                            int i = 1, int unused = -1);
    static void         setModelViewMatrix (int i = 1, int unused = -1);
    static void         doMouseTracking(bool on);
    static void         setMouseTrackingViewport(int x, int y, int w, int h);


protected:
    static std::ostream & debug();

protected:

    // Plain double-buffered OpenGL (2D)

    static void         displayBackBuffer(void *);
    static void *       backBufferUse();
    static void         backBufferUnuse(void *arg);

    // 2D rendering to framebuffer object then to OpenGL framebuffer.
    // (Enables antialised output on some platforms that do not support
    // OpenGL multisampling)

    struct BackBufferFBOParams
    {
        BackBufferFBOParams(int w, int h)
            : w(w), h(h), fbo(NULL)
        {
            fbo = new FrameInfo(w, h);
        }
        ~BackBufferFBOParams()
        {
            delete fbo;
        }
        void resize(int w, int h)
        {
            if (w != this->w || h != this->h)
            {
                fbo->resize(w, h);
                this->w = w;
                this->h = h;
            }
        }

        int          w, h;
        FrameInfo *  fbo;
    };
    static void         displayBackBufferFBO(void *);
    static void *       backBufferFBOUse();
    static void         backBufferFBOUnuse(void *arg);


    static double       stereoDelta(int i, int numCameras);

protected:
    bool                useFBO();

protected:
    struct DisplayParams
    {
        DisplayParams()
            : name(), fn(NULL), use(NULL), unuse(NULL),
              setopt(NULL), getopt(NULL), obj(NULL) {}
        DisplayParams(QString name,
                      ModuleApi::display_fn fn,
                      ModuleApi::display_use_fn use = NULL,
                      ModuleApi::display_unuse_fn unuse = NULL,
                      ModuleApi::display_setopt_fn setopt = NULL,
                      ModuleApi::display_getopt_fn getopt = NULL,
                      void * obj = NULL)
            : name(name), fn(fn), use(use), unuse(unuse),
              setopt(setopt), getopt(getopt), obj(obj) {}

        QString                       name;
        ModuleApi::display_fn         fn;
        ModuleApi::display_use_fn     use;
        ModuleApi::display_unuse_fn   unuse;
        ModuleApi::display_setopt_fn  setopt;
        ModuleApi::display_getopt_fn  getopt;
        void *                        obj;
    };
    typedef QMap<QString, DisplayParams>  display_map;

protected:
    DisplayParams         current;
    bool                  useInProgress;
    // Set by display module to adjust window_width and window_height
    double                wFactor, hFactor;
    static int            vpEvt;

protected:
    static display_map    map;
};

TAO_END

#endif // DISPLAY_DRIVER_H
