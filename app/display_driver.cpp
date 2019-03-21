// ****************************************************************************
//  display_driver.cpp                                             Tao project
// ****************************************************************************
//
//   File Description:
//
//    Implementation of the DisplayDriver class.
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
//  (C) 2011 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

#include "display_driver.h"
#include "widget.h"
#include "tao_gl.h"
#include "tao_utf8.h"
#include "base.h"
#include "application.h"
#include "window.h"
#include <iostream>


namespace Tao {

// Static map of registered display functions and associated parameters
DisplayDriver::display_map DisplayDriver::map;
int DisplayDriver::vpEvt = 0;


DisplayDriver::DisplayDriver()
// ----------------------------------------------------------------------------
//   Constructor
// ----------------------------------------------------------------------------
    : useInProgress(false), wFactor(1.0), hFactor(1.0)
{
    if (!vpEvt)
        vpEvt = QEvent::registerEventType();

    IFTRACE2(displaymode, layoutevents)
        debug() << "ID of 'viewpoints changed' user event: " << vpEvt << "\n";

    registerDisplayFunction("2Dplain", displayBackBuffer,
                                       NULL, NULL, NULL);

    registerDisplayFunction("2DFBO",   displayBackBufferFBO,
                                       backBufferFBOUse,
                                       backBufferFBOUnuse,
                                       NULL);

    text name2D = useFBO() ? "2DFBO" : "2Dplain";
    registerDisplayFunctionAlias("2D", name2D);
    registerDisplayFunctionAlias("default", "2D");

    // Function to be used initially
    setDisplayFunction("default");
}


DisplayDriver::~DisplayDriver()
// ----------------------------------------------------------------------------
//   Destructor
// ----------------------------------------------------------------------------
{
    IFTRACE(displaymode)
        debug() << "Deactivating display function: " << +current.name
                << "@" << (void*)current.fn << "\n";

    if (current.unuse)
        current.unuse(current.obj);
}


void DisplayDriver::display()
// ----------------------------------------------------------------------------
//   Use currently active rendering function, or default, to draw scene
// ----------------------------------------------------------------------------
{
    // Do not execute display callback if use callback has not returned yet.
    // Typical case: use() shows a message box, which causes the main event
    // loop to be re-entered, and thus display may be called again.
    if (useInProgress)
        return;
    XL_ASSERT(current.fn || !"No display function selected");
    return current.fn(current.obj);
}


bool DisplayDriver::setDisplayFunction(QString name)
// ----------------------------------------------------------------------------
//   Make a previously registered display function active
// ----------------------------------------------------------------------------
{
    bool found = false;
    if (name.isEmpty())
        name = "default";
    if (isCurrentDisplayFunctionSameAs(name))
        return true;
    if (map.contains(name))
    {
        DisplayParams save = current;

        if (current.unuse)
            current.unuse(current.obj);

        current = map[name];
        found = true;
        IFTRACE(displaymode)
            debug() << "Selecting display function: " << +name
                    << "@" << (void*)current.fn << "\n";
        if (current.use)
        {
            useInProgress = true;
            current.obj = current.use();
            useInProgress = false;
            if (!current.obj)
                current.obj = (void *) ~0UL;
        }

        if (current.obj == (void*)(~0L))
        {
            IFTRACE(displaymode)
                debug() << "Display function initialization error, "
                           "restoring previous function\n";
            current = save;
            if (current.use)
            {
                useInProgress = true;
                current.obj = current.use();
                useInProgress = false;
            }
            found = false;
        }
        else
        {
            // Defaults
            int planes;
            text t = getOption("PointsOfView", "1");
            planes = (+t).toInt();
            setStereoPlanes(planes);
            doMouseTracking(true);
            t = getOption("WindowWidthFactor", "1.0");
            wFactor = (+t).toDouble();
            t = getOption("WindowHeightFactor", "1.0");
            hFactor = (+t).toDouble();
        }
    }
    return found;
}


QString DisplayDriver::getDisplayFunction()
// ----------------------------------------------------------------------------
//   Return the name of the active display function
// ----------------------------------------------------------------------------
{
    return current.name;
}


bool DisplayDriver::isCurrentDisplayFunctionSameAs(QString name)
// ----------------------------------------------------------------------------
//   Check if currrent display function is the same as name
// ----------------------------------------------------------------------------
{
    if (name == "")
        name = "default";
    if (!map.contains(name))
        return false;
    return (current.fn == map[name].fn &&
            current.use == map[name].use);
}


bool DisplayDriver::setOption(std::string name, std::string val)
// ----------------------------------------------------------------------------
//   Pass option to display module
// ----------------------------------------------------------------------------
{
    bool ok = false;
    IFTRACE(displaymode)
        debug() << "Passing option to display function: " << +current.name
                << " \"" << name << "\"=\"" << val << "\"\n";
    if (current.setopt)
        ok = current.setopt(current.obj, name, val);
    if (!ok)
    {
        IFTRACE(displaymode)
            debug() << "Option not recognized or rejected\n";
    }
    return ok;
}


std::string DisplayDriver::getOption(std::string name, std::string deflt)
// ----------------------------------------------------------------------------
//   Read option from display module
// ----------------------------------------------------------------------------
{
    std::string val, defstr;
    if (current.getopt)
        val = current.getopt(current.obj, name);
    if (val == "")
    {
        IFTRACE(displaymode)
            defstr = " (from default)";
        val = deflt;
    }
    IFTRACE(displaymode)
        debug() << "Read option from display function: " << +current.name
                << " \"" << name << "\"=\"" << val << "\"" << defstr << "\n";
    return val;
}


QStringList DisplayDriver::allDisplayFunctions()
// ----------------------------------------------------------------------------
//   Return all registered function names
// ----------------------------------------------------------------------------
{
    return map.keys();
}


bool DisplayDriver::useFBO()
// ----------------------------------------------------------------------------
//   Shall we do 2D rendering through a framebuffer object (true), or directly?
// ----------------------------------------------------------------------------
{
    IFTRACE(displaymode)
    {
        text gl, fbos;
        if (!TaoApp->hasGLMultisample)
            gl = "not ";
        if (!TaoApp->hasFBOMultisample)
            fbos = "not ";
        debug() << "GL framebuffer multisampling: " << gl << "supported\n";
        debug() << "FBOs multisampling: " << fbos << "supported\n";
    }
    return (!TaoApp->hasGLMultisample && TaoApp->hasFBOMultisample);
}


std::ostream & DisplayDriver::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[Display Driver] ";
    return std::cerr;
}



// ============================================================================
//
//   Static display methods. To be later moved to modules, except default one.
//
// ============================================================================

void DisplayDriver::displayBackBuffer(void *)
// ----------------------------------------------------------------------------
//   Default, usual 2D rendering into OpenGL back buffer
// ----------------------------------------------------------------------------
{
    // Save GL state
    Tao::GraphicSave* save = GL.Save();

    // Are we rendering to the default framebuffer, or a FBO?
    GLint fbname = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbname);

    // Setup viewport
    int w = renderWidth();
    int h = renderHeight();
    GL.Viewport(0, 0, w, h);

    // Setup projection and modelview matrices
    setProjectionMatrix(w, h);
    setModelViewMatrix();

    // If no FBO is bound, select draw buffer
    if (!fbname)
        GL.DrawBuffer(GL_BACK);

    // Clear color and depth information
    setGlClearColor();
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set suitable GL parameters for drawing
    setupGl();

    // Draw scene, selection, and activities
    drawScene();
    drawSelection();
    drawActivities();

    GL.Restore(save);
}


void DisplayDriver::displayBackBufferFBO(void *obj)
// ----------------------------------------------------------------------------
//   Like displayBackBuffer, but first render into a framebuffer object.
// ----------------------------------------------------------------------------
//   This function enables better antialiasing on platforms that do not have
//   multisample GL widget, but do support multisample FBOs.
{
    // Save graphic state
    Tao::GraphicSave *save = GL.Save();

    BackBufferFBOParams * o = (BackBufferFBOParams *)obj;
    XL_ASSERT(obj || !"Back buffer FBO display routine received NULL object");

    // Read output resolution
    int w = renderWidth();
    int h = renderHeight();

    // Make sure output buffer has the right size (resolution may have changed)
    // and prepare to draw into it
    o->resize(w, h);
    o->fbo->begin();

    GL.Viewport(0, 0, w, h);

    // Setup projection and modelview matrices
    setProjectionMatrix(w, h);
    setModelViewMatrix();

    // Clear color and depth information
    setGlClearColor();
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set suitable GL parameters for drawing
    setupGl();

    // Draw scene, selection, and activities
    drawScene();
    drawSelection();
    drawActivities();

    // Done with drawing.
    // Make buffer available as a texture.
    o->fbo->end();
    o->fbo->bind();

    // Draw a full-screen textured quad

    // Setup viewport and geometry
    GL.Viewport(0, 0, w, h);
    GL.MatrixMode(GL_PROJECTION);
    GL.LoadIdentity();
    GL.MatrixMode(GL_MODELVIEW);
    GL.LoadIdentity();
    GL.LoadMatrix();

    // Select draw buffer
    GL.DrawBuffer(GL_BACK);

    // Clear depth information, disable color blending so that texture alpha
    // is ignored
    GL.Clear(GL_DEPTH_BUFFER_BIT);
    GL.Disable(GL_BLEND);

    // Not sure why, but without this I often have a blank screen
    GL.Disable(GL_POLYGON_OFFSET_FILL);
    GL.Disable(GL_POLYGON_OFFSET_LINE);
    GL.Disable(GL_POLYGON_OFFSET_POINT);

    GL.Begin(GL_QUADS);
    GL.TexCoord( 0 , 0);
    GL.Vertex   (-1, -1);
    GL.TexCoord( 1 , 0);
    GL.Vertex   ( 1, -1);
    GL.TexCoord( 1,  1);
    GL.Vertex   ( 1,  1);
    GL.TexCoord( 0,  1);
    GL.Vertex   (-1,  1);
    GL.End();

    // Restore state
    GL.Restore(save);
}


void * DisplayDriver::backBufferFBOUse()
// ----------------------------------------------------------------------------
//   2D back buffer FBO display is about to be used: allocate context
// ----------------------------------------------------------------------------
{
    int w = renderWidth(), h = renderHeight();
    return new BackBufferFBOParams(w, h);
}


void DisplayDriver::backBufferFBOUnuse(void *obj)
// ----------------------------------------------------------------------------
//   Done with 2D back buffer FBO display
// ----------------------------------------------------------------------------
{
    delete (BackBufferFBOParams *)obj;
}


// ============================================================================
//
//   Static methods exported to module API
//
// ============================================================================


bool DisplayDriver::registerDisplayFunction(std::string name,
                                            ModuleApi::display_fn fn,
                                            ModuleApi::display_use_fn use,
                                            ModuleApi::display_unuse_fn unuse,
                                            ModuleApi::display_setopt_fn setopt,
                                            ModuleApi::display_getopt_fn getopt)
// ----------------------------------------------------------------------------
//   Add a display function to the list of known functions
// ----------------------------------------------------------------------------
{
    IFTRACE(displaymode)
        debug() << "Registering display function: " << name << "@"
                << (void*)fn << "\n";
    QString nam = +name;
    if (map.contains(nam))
    {
        IFTRACE(displaymode)
            debug() << "Error: name already in use\n";
        return false;
    }
    DisplayParams p(nam, fn, use, unuse, setopt, getopt);
    map[nam] = p;
    return true;
}


bool DisplayDriver::registerDisplayFunctionAlias(std::string name,
                                                 std::string other)
// ----------------------------------------------------------------------------
//   Create another name for an existing display function.
// ----------------------------------------------------------------------------
{
    QString nam = +name, oth = +other;
    if (map.contains(nam))
    {
        IFTRACE(displaymode)
            debug() << "Error: name " << name << " already in use\n";
        return false;
    }
    if (!map.contains(oth))
    {
        IFTRACE(displaymode)
            debug() << "Error: name " << other << " not found\n";
        return false;
    }

    DisplayParams p = map[nam] = map[oth];

    IFTRACE(displaymode)
        debug() << "Registering alias: " << name
                << " => " << other << "@" << (void*)p.fn << "\n";
    return true;
}


void DisplayDriver::drawScene()
// ----------------------------------------------------------------------------
//   Draw all objects in the scene
// ----------------------------------------------------------------------------
{
    Widget::Tao()->drawScene();
}


void DisplayDriver::drawSelection()
// ----------------------------------------------------------------------------
//   Draw selection items for all objects (selection boxes, manipulators)
// ----------------------------------------------------------------------------
{
    Widget::Tao()->drawSelection();
}


void DisplayDriver::drawActivities()
// ----------------------------------------------------------------------------
//   Draw chooser, selection rectangle
// ----------------------------------------------------------------------------
{
    Widget::Tao()->drawActivities();
}


void DisplayDriver::setGlClearColor()
// ----------------------------------------------------------------------------
//   Clear color with the color specified in the widget
// ----------------------------------------------------------------------------
{
    Widget::Tao()->setGlClearColor();
}


void DisplayDriver::setupGl()
// ----------------------------------------------------------------------------
//   Set default GL parameters before drawing the scene
// ----------------------------------------------------------------------------
{
    Widget::Tao()->setupGL();
}


void DisplayDriver::showGlErrors()
// ----------------------------------------------------------------------------
//   Display all OpenGL errors in the error window
// ----------------------------------------------------------------------------
{
    Widget::Tao()->showGlErrors();
}


bool DisplayDriver::setStereo(bool on)
// ----------------------------------------------------------------------------
//   Enable or disable quad buffers for current widget
// ----------------------------------------------------------------------------
{
    return Widget::Tao()->taoWindow()->setStereo(on);
}


void DisplayDriver::getCamera(Point3 *pos, Point3 *target, Vector3 *up,
                              double *toScreen)
// ----------------------------------------------------------------------------
//   Get camera characteristics
// ----------------------------------------------------------------------------
{
    Widget::Tao()->getCamera(pos, target, up, toScreen);
}


static inline int even(int x)
// ----------------------------------------------------------------------------
//   Smallest even integer larger or equal to x
// ----------------------------------------------------------------------------
{
    return (x+1) & ~1;
}


int DisplayDriver::renderHeight()
// ----------------------------------------------------------------------------
//   Window height for rendering purposes
// ----------------------------------------------------------------------------
//   Dimension in pixels is rounded down to an even value to avoid display
//   artifacts as the window is resized (half pixels)
{
    int h = Widget::Tao()->renderHeight();
    return even(h);
}


int DisplayDriver::renderWidth()
// ----------------------------------------------------------------------------
//   Window width for rendering purposes
// ----------------------------------------------------------------------------
//   Dimension in pixels is rounded down to an even value to avoid display
//   artifacts as the window is resized (half pixels)
{
    int w = Widget::Tao()->renderWidth();
    return even(w);
}


double DisplayDriver::zNear()
// ----------------------------------------------------------------------------
//   Near plane
// ----------------------------------------------------------------------------
{
    return Widget::Tao()->zNear;
}


double DisplayDriver::zFar()
// ----------------------------------------------------------------------------
//   Far plane
// ----------------------------------------------------------------------------
{
    return Widget::Tao()->zFar;
}


double DisplayDriver::zoom()
// ----------------------------------------------------------------------------
//   Current zoom factor
// ----------------------------------------------------------------------------
{
    Widget *widget = Widget::Tao();
    return widget->zoom * widget->devicePixelRatio;
}


double DisplayDriver::eyeSeparation()
// ----------------------------------------------------------------------------
//   Distance between eyes
// ----------------------------------------------------------------------------
{
    return Widget::Tao()->eyeDistance;
}


int DisplayDriver::getCurrentEye()
// ----------------------------------------------------------------------------
//   Current eye
// ----------------------------------------------------------------------------
{
    return Widget::Tao()->stereoPlane + 1;
}


int DisplayDriver::getEyesNumber()
// ----------------------------------------------------------------------------
//   Number of eyes
// ----------------------------------------------------------------------------
{
    return Widget::Tao()->stereoPlanes;
}


void DisplayDriver::setStereoPlanes(uint planes)
// ----------------------------------------------------------------------------
//   Set the number of views per frame
// ----------------------------------------------------------------------------
{
    uint &current = Widget::Tao()->stereoPlanes;
    if (current != planes)
    {
        current = planes;
        Widget::postEventAPI(viewPointsChangedEvent());
    }
}


void DisplayDriver::doMouseTracking(bool on)
// ----------------------------------------------------------------------------
//   Is current view useable for selection?
// ----------------------------------------------------------------------------
{
    Widget::Tao()->doMouseTracking = on;
}


void DisplayDriver::setMouseTrackingViewport(int x, int y, int w, int h)
// ----------------------------------------------------------------------------
//   Set the viewport that will be used to project mouse cursor
// ----------------------------------------------------------------------------
{
    GLint *v = Widget::Tao()->mouseTrackingViewport;
    v[0] = x; v[1] = y; v[2] = w; v[3] = h;
}


void DisplayDriver::setProjectionMatrix(int w, int h, int i, int)
// ----------------------------------------------------------------------------
//   Set frustum for the given camera
// ----------------------------------------------------------------------------
{
    // Record which stereo plane we are on for stereo
    Widget::Tao()->stereoPlane = i-1;
    int numCameras = Widget::Tao()->stereoPlanes;

    // Read camera distance to screen
    double toScreen;
    getCamera(NULL, NULL, NULL, &toScreen);

    // Setup the projection matrix
    GL.MatrixMode(GL_PROJECTION);
    GL.LoadIdentity();
    double nearRatio = zNear()/toScreen;
    double delta = stereoDelta(i, numCameras);
    double shift = -eyeSeparation() * delta * nearRatio;
    double f = 0.5 * nearRatio / zoom();
    GL.Frustum(-w*f + shift, w*f + shift, -h*f, h*f, zNear(), zFar());
    GL.LoadMatrix();
}


void DisplayDriver::setModelViewMatrix(int i, int)
// ----------------------------------------------------------------------------
//   Set modelview matrix for the given camera
// ----------------------------------------------------------------------------
{
    // Record which stereo plane we are on for stereo
    Widget::Tao()->stereoPlane = i-1;
    int numCameras = Widget::Tao()->stereoPlanes;

    // Read camera position
    Point3 cameraPosition;
    Point3 cameraTarget;
    Vector3 cameraUpVector;
    double toScreen;
    getCamera(&cameraPosition, &cameraTarget, &cameraUpVector, &toScreen);

    // Setup the model-view matrix
    GL.MatrixMode(GL_MODELVIEW);
    GL.LoadIdentity();
    double delta = stereoDelta(i, numCameras);
    double shiftLength = eyeSeparation() * delta;
    Vector3 toTarget = Vector3(cameraTarget - cameraPosition).Normalize();
    toTarget *= toScreen;
    Point3 target = cameraPosition + toTarget;
    Vector3 shift = toTarget.Cross(cameraUpVector).Normalize() * shiftLength;

    GL.LookAt(cameraPosition.x + shift.x,
              cameraPosition.y + shift.y,
              cameraPosition.z + shift.z,
              target.x + shift.x,
              target.y + shift.y,
              target.z + shift.z,
              cameraUpVector.x, cameraUpVector.y, cameraUpVector.z);
}


double DisplayDriver::stereoDelta(int i, int numCameras)
// ----------------------------------------------------------------------------
//  Compute x-axis shift for the given camera (multiples of eye distance)
// ----------------------------------------------------------------------------
//  Examples: with 3 cameras: -1, 0, 1
//            with 4 cameras: -1.5, -0.5, 0.5, 1.5
{
    // First camera is i=1
    i--;
    i = i - numCameras/2;
    double delta = i;
    if (numCameras % 2 == 1)
        delta -= 0.5;
    delta += 0.5;
    return delta;
}


}
