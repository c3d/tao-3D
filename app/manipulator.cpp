// ****************************************************************************
//  manipulator.cpp                                                 Tao project
// ****************************************************************************
//
//   File Description:
//
//    Helper class used to assign GL names to individual graphic shapes
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
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "manipulator.h"
#include "drag.h"
#include "layout.h"
#include "shapes.h"
#include "widget_surface.h"
#include "path3d.h"
#include "gl_keepers.h"
#include "runtime.h"
#include "transforms.h"
#include "apply_changes.h"
#include "table.h"
#include <cmath>

TAO_BEGIN

// ============================================================================
//
//    Simple manipulator
//
// ============================================================================

Manipulator::Manipulator(XL::Tree *self)
// ----------------------------------------------------------------------------
//   Record the GL name for a given tree
// ----------------------------------------------------------------------------
    :self(self)
{}


void Manipulator::Draw(Layout *layout)
// ----------------------------------------------------------------------------
//   When drawing a manipulator, we don't draw anything
// ----------------------------------------------------------------------------
{
    (void) layout;
}


void Manipulator::DrawSelection(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the manipulator using simple GL points
// ----------------------------------------------------------------------------
{
    Widget *widget = layout->Display();
    uint sel = widget->selected(layout);
    if (sel && layout->transparency)
    {
        widget->selectionTrees.insert(self);
        GL.PushName(layout->id);
        DrawHandles(layout);
        GL.PopName();
    }
}


void Manipulator::Identify(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the manipulator selection handles
// ----------------------------------------------------------------------------
{
    Manipulator::DrawSelection(layout);
}


XL::Tree *Manipulator::Source()
// ----------------------------------------------------------------------------
//   Return the source tree for this manipulator
// ----------------------------------------------------------------------------
{
    return self;
}


bool Manipulator::DrawHandle(Layout *layout, Point3 p, uint id, text name)
// ----------------------------------------------------------------------------
//   Draw one of the handles for the current manipulator
// ----------------------------------------------------------------------------
{
    Widget  *widget   = layout->Display();
    Vector3  offset   = layout->Offset();
    bool     selected = false;
    if (layout->groupDrag)
    {
        widget->drawHandle(layout, p + offset, "group_" + name, id);
    }
    else
    {
        widget->drawHandle(layout, p + offset, name, id);
        selected = widget->selectionHandleId() == id;
    }
    return selected;
}


void Manipulator::updateArg(Widget *, Tree *arg,
                            double first, double previous, double current,
                            double min, double max)
// ----------------------------------------------------------------------------
//   Update the given argument by the given offset
// ----------------------------------------------------------------------------
{
    // Verify if there is something we are allowed to modify
    if (!arg || previous == current || IsMarkedConstant(arg))
        return;

    Tree_p  source   = xl_source(arg); // Find the source expression
    Tree_p *ptr      = &source;
    bool    more     = true;
    double  scale    = 1.0;

    // Check if we have an Infix +, if so walk down the left side
    arg = source;
    while (more)
    {
        more = false;
        if (XL::Infix *infix = (*ptr)->AsInfix())
        {
            if (infix->name == "-")
            {
                ptr = &infix->left;
                more = true;
            }
            else if (infix->name == "+")
            {
                ptr = &infix->left;
                more = true;
            }
            else if (infix->name == "*")
            {
                if (XL::Real *lr = infix->left->AsReal())
                {
                    scale *= lr->value;
                    ptr = &infix->right;
                    more = true;
                }
                else if (XL::Real *rr = infix->right->AsReal())
                {
                    scale *= rr->value;
                    ptr = &infix->left;
                    more = true;
                }
                else if (XL::Integer *li = infix->left->AsInteger())
                {
                    scale *= li->value;
                    ptr = &infix->right;
                    more = true;
                }
                else if (XL::Integer *ri = infix->right->AsInteger())
                {
                    scale *= ri->value;
                    ptr = &infix->left;
                    more = true;
                }
            }
        }
        if (XL::Prefix *prefix = (*ptr)->AsPrefix())
        {
            if (XL::Name *name = prefix->left->AsName())
            {
                if (name->value == "-")
                {
                    ptr = &prefix->right;
                    more = true;
                    scale = -scale;
                }
            }
        }
        if (XL::Postfix *postfix = (*ptr)->AsPostfix())
        {
            if (XL::Name *name = postfix->right->AsName())
            {
                if (name->value == "%")
                {
                    ptr = &postfix->left;
                    more = true;
                    scale /= 100;
                }
            }
        }
    }
    // REVISIT: Really?
    if (scale == 0.0)
        scale = 1.0;

    // Test the simple cases where the argument is directly an Integer or Real
    if (XL::Integer *ival = (*ptr)->AsInteger())
    {
        ival->value -= longlong((previous - first) / scale);
        ival->value += longlong((current - first) / scale);
        if (ival->value * scale < min)
            ival->value = min / scale;
        if (ival->value * scale > max)
            ival->value = max / scale;
    }
    else if (XL::Real *rval = (*ptr)->AsReal())
    {
        rval->value += (current - previous) / scale;
        if (rval->value * scale < min)
            rval->value = min / scale;
        if (rval->value * scale > max)
            rval->value = max / scale;
    }
    else
    {
        // Create an Infix + with the delta we add
        // LIONEL: When does that happen?
        // CHRISTOPHE: The first time we hit an expression which is not
        // a linear operation on the argument
        if (ptr != &source)
        {
            double value = current;
            if (current < min)
                value = min;
            if (current > max)
                value = max;
            double delta = (value - previous) / scale;
            *ptr = new XL::Infix("+", new XL::Real(delta), *ptr);
        }
    }
}


void Manipulator::rotate(Widget *widget, Tree *shape, kPoint3 center,
                         kPoint3 p0, kPoint3 p1, kPoint3 p2, bool stepped)
// ----------------------------------------------------------------------------
//   Rotate the shape around the given center, given 3 drag points
// ----------------------------------------------------------------------------
//   We create the necessary rotatez and translate statements
{
    Widget::attribute_args rArgs, tArgs;
    TreeList setTArgs, setRArgs;
    double a1 = 0.0, tx = 0.0, ty = 0.0, tz = 0.0;
    double cx = center.x, cy = center.y;

    // Get current rotation and translation
    if (widget->get(shape, "translate", tArgs) && tArgs.size() == 3)
    {
        tx = tArgs[0];
        ty = tArgs[1];
        tz = tArgs[2];
    }
    if (widget->get(shape, "rotatez", rArgs) && rArgs.size() == 1)
        a1 = rArgs[0];

    // Compute new rotation angle
    double da1 = atan2(p1.y - center.y, p1.x - center.x) * (180 / M_PI);
    double da2 = atan2(p2.y - center.y, p2.x - center.x) * (180 / M_PI);
    double a2 = fmod(a1 - da1 + da2 + 360, 360);
    if (stepped)
    {
        double da0 = atan2(p0.y - center.y, p0.x - center.x) * (180 / M_PI);

        int n1 = (da1 - da0 + (2*360 - 22.5)) / 45;
        int n2 = (da2 - da0 + (2*360 - 22.5)) / 45;

        a2 = 45 * int(a1/45);
        if (n2 > n1)
            a2 += 45;
        else if (n2 < n1)
            a2 -= 45;
    }

    // If c is the rotation center and s the shape position
    //   x' = tx + cx * cos a - cy * sin a
    //   y' = ty + cx * sin a + cy * cos a
    // We compute tx and ty so that we preserve cx and cy when a changes:
    //   tx1 + cx * cos a1 - cy * sin a1
    // = tx2 + cx * cos a2 + cy * sin a2
    double ca1 = cos(a1 * (M_PI/180)), sa1 = sin(a1 * (M_PI/180));
    double ca2 = cos(a2 * (M_PI/180)), sa2 = sin(a2 * (M_PI/180));
    tx -= cx*(ca2-ca1) - cy*(sa2-sa1);
    ty -= cx*(sa2-sa1) + cy*(ca2-ca1);

    // Update translation and rotation
    setRArgs.resize(1);
    setRArgs[0] = new XL::Real(a2);
    widget->set(shape, "rotatez", setRArgs);
    setTArgs.resize(3);
    setTArgs[0] = new XL::Real(tx);
    setTArgs[1] = new XL::Real(ty);
    setTArgs[2] = new XL::Real(tz);
    widget->set(shape, "translate", setTArgs);
}



// ============================================================================
//
//    A control point updates specific coordinates
//
// ============================================================================

ControlPoint::ControlPoint(Tree *self, bool onCurve,
                           Real *x, Real *y, Real *z, uint id)
// ----------------------------------------------------------------------------
//   Record where we want to draw
// ----------------------------------------------------------------------------
    : Manipulator(self), x(x), y(y), z(z), id(id + 4), onCurve(onCurve)
{}


void ControlPoint::Draw(Layout *where)
// ----------------------------------------------------------------------------
//    Draw a control point
// ----------------------------------------------------------------------------
{
    Manipulator::Draw(where);
}


void ControlPoint::DrawSelection(Layout *layout)
// ----------------------------------------------------------------------------
//    Draw the selection for a control point
// ----------------------------------------------------------------------------
{
    // We don't need to glPushName, as the parent should have done it for us
    Widget *widget = layout->Display();
    uint sel = widget->selected(layout);
    if (sel)
    {
        widget->selectionTrees.insert(self);
        DrawHandles(layout);
    }
}


void ControlPoint::Identify(Layout *where)
// ----------------------------------------------------------------------------
//    Select the identity for a control point
// ----------------------------------------------------------------------------
{
    ControlPoint::DrawSelection(where);
}


bool ControlPoint::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   For a control point, there is a single handle
// ----------------------------------------------------------------------------
{
    if (!IsMarkedConstant(x) || !IsMarkedConstant(y) || !IsMarkedConstant(z))
    {
        if (layout->groupDrag ||
            DrawHandle(layout, Point3(x, y, z), id,
                       onCurve?"curve_point_handle":"control_point_handle"))
        {
            Widget *widget = layout->Display();
            Drag *drag = widget->drag();
            if (drag)
            {
                Point3 p1 = drag->Previous();
                Point3 p2 = drag->Current();
                if (p1 != p2 && widget->markChange("Control point moved"))
                {
                    Point3 p0 = drag->Origin();
                    updateArg(widget, x,  p0.x, p1.x, p2.x);
                    updateArg(widget, y,  p0.y, p1.y, p2.y);
                    updateArg(widget, z,  p0.z, p1.z, p2.z);
                    return true;
                }
            }
        }
    }
    return false;
}



// ============================================================================
//
//   A frame manipulator allows click-through for rectangular shapes
//
// ============================================================================

FrameManipulator::FrameManipulator(Tree *self,
                                   Real *x, Real *y, Real *w, Real *h)
// ----------------------------------------------------------------------------
//   A control rectangle owns a given child and manipulates it
// ----------------------------------------------------------------------------
    : Manipulator(self), x(x), y(y), w(w), h(h)
{}


void FrameManipulator::DrawSelection(Layout *layout)
// ----------------------------------------------------------------------------
//  Draw a rectangular selection unless we double-clicked to edit inside
// ----------------------------------------------------------------------------
{
    Widget *widget = layout->Display();
    uint sel = widget->selected(layout);
    if (sel)
        Manipulator::DrawSelection(layout);
}


bool FrameManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles around a widget
// ----------------------------------------------------------------------------
{
    Widget *widget = layout->Display();
    coord   xx = x, yy = y, ww = w, hh = h;
    Drag   *drag = widget->drag();
    uint    handle = 0;
    uint    selected = widget->selected(layout);

    // Don't draw the handles if this is an open container
    if ((selected & Widget::SELECTION_MASK) == Widget::CONTAINER_OPENED)
        return false;

    int mode = CurrentTransformMode();
    text change = (mode & TM_ROTATE_BIT) ? "rotate" : "resize";
    bool canChange = widget->markChange("Shape " + change);

    for (uint hn = 0; hn < 4; hn++)
    {
        double sw = (hn & 1) ? ScaleX() : -ScaleX();  // sh < 0 ? lower : upper
        double sh = (hn & 2) ? ScaleY() : -ScaleY();  // sw < 0 ? left : right

        if (!DrawHandle(layout, Point3(xx+ww/(2*sw), yy+hh/(2*sh), 0), hn+1))
            continue;
        handle = hn+1;

        // Update arguments if necessary
        if (!drag || !canChange)
            continue;

        Point3 p1 = drag->Previous();
        Point3 p2 = drag->Current();
        if (p1 == p2)
            continue;

        Point3 p0 = drag->Origin();

        switch (mode)
        {
        case TM_ResizeLockCenter:
            updateArg(widget, w, 2*sw*p0.x, 2*sw*p1.x, 2*sw*p2.x);
            updateArg(widget, h, 2*sh*p0.y, 2*sh*p1.y, 2*sh*p2.y);
            break;

        case TM_FreeCenteredRotate:
        case TM_SteppedCenteredRotate:
            rotate(widget, self, Point3(xx, yy, 0), p0, p1, p2,
                   mode == TM_SteppedCenteredRotate);
            break;

        case TM_FreeOppositeRotate:
        case TM_SteppedOppositeRotate:
            rotate(widget, self,
                   Point3(xx-sw*w/2, yy-sh*h/2, 0),
                   p0, p1, p2, mode == TM_SteppedOppositeRotate);
            break;

        case TM_ResizeLockAspectRatio:
            {
                int id = widget->selectionCurrentId();
                coord &h0 = drag->h0[id];
                coord &w0 = drag->w0[id];
                if (!h0 && !w0)
                {
                    // Starting drag action: save shape size
                    if (!h || !w)
                        break;
                    h0 = h; w0 = w;
                }

                scale r = w0/h0;
                coord ux, uy, uw, uh;
                coord a, b, c;
                bool condY1, condY2;
                condY1 = (fabs(p1.y - y)/h0 > fabs(p1.x - x)/w0);
                condY2 = (fabs(p2.y - y)/h0 > fabs(p2.x - x)/w0);
                if (condY1 != condY2)
                {
                    // Pointer crossed diagonal
                    // Set params back to p0 state to avoid accumulation of
                    // rounding errors
                    if (condY1)
                    {
                        ux = r*sh*sw/2; uy = 0.5; uw = r*sh; uh = sh;
                        a = p0.y; b = p1.y ; c = p0.y;
                    }
                    else
                    {
                        ux = 0.5; uy = (1/r)*sh*sw/2; uw = sw; uh = (1/r)*sw;
                        a = p0.x; b = p1.x ; c = p0.x;
                    }
                    updateArg(widget, x, ux*a, ux*b, ux*c);
                    updateArg(widget, y, uy*a, uy*b, uy*c);
                    updateArg(widget, w, uw*a, uw*b, uw*c);
                    updateArg(widget, h, uh*a, uh*b, uh*c);
                    p1 = p0;
                }
                if (condY2)
                {
                    ux = r*sh*sw/2; uy = 0.5; uw = r*sh; uh = sh;
                    a = p0.y; b = p1.y ; c = p2.y;
                }
                else
                {
                    ux = 0.5; uy = (1/r)*sh*sw/2; uw = sw; uh = (1/r)*sw;
                    a = p0.x; b = p1.x ; c = p2.x;
                }
                updateArg(widget, x, ux*a, ux*b, ux*c);
                updateArg(widget, y, uy*a, uy*b, uy*c);
                updateArg(widget, w, uw*a, uw*b, uw*c);
                updateArg(widget, h, uh*a, uh*b, uh*c);
                break;
            }

        case TM_ResizeLockCenterAndAspectRatio:
            // TODO

        case TM_FreeResize:
        default:
            updateArg(widget, x, p0.x/2, p1.x/2, p2.x/2);
            updateArg(widget, y, p0.y/2, p1.y/2, p2.y/2);
            updateArg(widget, w, sw*p0.x, sw*p1.x, sw*p2.x);
            updateArg(widget, h, sh*p0.y, sh*p1.y, sh*p2.y);
            break;
        }
    }

    return handle != 0;
}


Box3 FrameManipulator::Bounds(Layout *where)
// ----------------------------------------------------------------------------
//   Return the bounding box for the shape
// ----------------------------------------------------------------------------
{
    Point3  l(x-w/2, y-h/2, 0);
    Vector3 s(w, h, 0);
    return Box3(l + where->offset, s);
}


FrameManipulator::TransformMode FrameManipulator::CurrentTransformMode()
// ----------------------------------------------------------------------------
//   Define editing constraints depending on current keyboard modifier keys
// ----------------------------------------------------------------------------
{
    int m = (int)QApplication::keyboardModifiers();
    switch (m & (Qt::AltModifier + Qt::ShiftModifier + Qt::ControlModifier))
    {
    case (Qt::AltModifier + Qt::ShiftModifier):
        return TM_ResizeLockCenterAndAspectRatio;
    case (Qt::AltModifier):
        return TM_ResizeLockCenter;
    case (Qt::ShiftModifier):
        return TM_ResizeLockAspectRatio;
    case (Qt::ControlModifier + Qt::AltModifier + Qt::ShiftModifier):
        return TM_SteppedOppositeRotate;
    case (Qt::ControlModifier + Qt::AltModifier):
        return TM_FreeOppositeRotate;
    case (Qt::ControlModifier + Qt::ShiftModifier):
        return TM_SteppedCenteredRotate;
    case (Qt::ControlModifier):
        return TM_FreeCenteredRotate;
    default:
        break;
    }

    return TM_FreeResize;
}



// ============================================================================
//
//   A rectangle manipulator udpates x, y, w and h and allows translation
//
// ============================================================================

ControlRectangle::ControlRectangle(Tree *self,
                                   Real *x, Real *y, Real *w, Real *h)
// ----------------------------------------------------------------------------
//   A control rectangle owns a given child and manipulates it
// ----------------------------------------------------------------------------
    : FrameManipulator(self, x, y, w, h)
{}


bool ControlRectangle::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for a rectangular object
// ----------------------------------------------------------------------------
{
    // Check if we clicked anywhere else in the shape
    bool changed = FrameManipulator::DrawHandles(layout);
    if (!changed)
    {
        Widget *widget = layout->Display();
        uint    selected = widget->selected(layout);
        // Don't move if this is an open container BUG#891
        if ((selected & Widget::SELECTION_MASK) == Widget::CONTAINER_OPENED)
            return true;

        Drag *drag = widget->drag();
        if (drag && (layout->groupDrag || !widget->selectionHandleId()))
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Shape moved"))
            {
                Point3 p0 = drag->Origin();
                updateArg(widget, x, p0.x, p1.x, p2.x);
                updateArg(widget, y, p0.y, p1.y, p2.y);
                changed = true;
            }
        }
    }
    return changed;
}



// ============================================================================
//
//   A rounded rectangle manipulator udpates x, y, w, h and corner radius
//
// ============================================================================

ControlRoundedRectangle::ControlRoundedRectangle(Tree *self,
                                                 Real *x, Real *y,
                                                 Real *w, Real *h,
                                                 Real *r)
// ----------------------------------------------------------------------------
//   A control arrow adds the radius of the corners to the control rectangle
// ----------------------------------------------------------------------------
    : ControlRectangle(self, x, y, w, h), r(r)
{}


bool ControlRoundedRectangle::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for the rounded rectangle (assuming rx = ry)
// ----------------------------------------------------------------------------
{
    bool changed = ControlRectangle::DrawHandles(layout);

    Widget *widget = layout->Display();
    Drag   *drag = widget->drag();
    coord rr = ((double) r < 0 ? 0 : 1) * r;
    int sw = w > 0? 1: -1;
    int sh = h > 0? 1: -1;

    Point3 handle;
    int id = 9;

    if (IsMarkedConstant(r))
        return changed;

    if (sw*w < sh*h)
    {
        if (r > sh*h/2)
            rr = sh*h/2;

        handle = Point3(x + sw*w/2,y + sh*h/2 - rr, 0);
        if (DrawHandle(layout, handle, id, "adjust_shape_handle"))
        {
            if (drag)
            {
                Point3 p1 = drag->Previous();
                Point3 p2 = drag->Current();
                if (p1 != p2 &&
                    widget->markChange("Rounded rectangle corner modified"))
                {
                    Point3 p0 = drag->Origin();
                    updateArg(widget, r,
                              y+sh*h/2-p0.y, y+sh*h/2-p1.y, y+sh*h/2-p2.y,
                              0.0, sh*h/2);
                    changed = true;
                }
            }
        }
    }
    else
    {
        if (r > sw*w/2)
            rr = sw*w/2;

        handle = Point3(x + sw*w/2 - rr, y + sh*h/2, 0);
        if (DrawHandle(layout, handle, id, "adjust_shape_handle"))
        {
            if (drag)
            {
                Point3 p1 = drag->Previous();
                Point3 p2 = drag->Current();
                if (p1 != p2 &&
                    widget->markChange("Rounded rectangle corner modified"))
                {
                    Point3 p0 = drag->Origin();
                    updateArg(widget, r,
                              x+sw*w/2-p0.x, x+sw*w/2-p1.x, x+sw*w/2-p2.x,
                              0.0, sw*w/2);
                    changed = true;
                }
            }
        }
    }

    return changed;
}



// ============================================================================
//
//   An arrow manipulator udpates x, y, w, h, and the arrow handle a
//
// ============================================================================

ControlArrow::ControlArrow(Tree *self,
                           Real *x, Real *y, Real *w, Real *h,
                           Real *ax, Real *ary)
// ----------------------------------------------------------------------------
//   Same as above setting is_double to false
// ----------------------------------------------------------------------------
    : ControlRectangle(self, x, y, w, h), ax(ax), ary(ary), d(false)
{}


ControlArrow::ControlArrow(Tree *self,
                           Real *x, Real *y, Real *w, Real *h,
                           Real *ax, Real *ary, bool is_double)
// ----------------------------------------------------------------------------
//   A control arrow adds the arrow handle to the control rectangle
// ----------------------------------------------------------------------------
    : ControlRectangle(self, x, y, w, h),
      ax(ax), ary(ary), d(is_double)
{}


bool ControlArrow::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for an arrow
// ----------------------------------------------------------------------------
{
    bool changed = ControlRectangle::DrawHandles(layout);

    if (IsMarkedConstant(ax) && IsMarkedConstant(ary))
        return changed;

    coord aax, aay;
    int sw = w > 0? 1: -1;
    int sdw = d? sw: 1;
    int swd = d? 1: sw;
    int sh = h > 0? 1: -1;
    int df = d? 2: 1;

    if (ax > sw*w/df)
        aax = w/df;
    else
        aax = sw*ax;

    if (ax < 0.0)
        aax = 0.0;

    if (ary > 1.0)
        aay = h;
    else
        aay = ary*h;

    if (ary < 0.0)
        aay = 0.0;

    Point3 handle = Point3(x+sdw*(w/2-aax), y+sh*aay/2, 0);
    if (DrawHandle(layout, handle, 9, "adjust_shape_handle"))
    {
        Widget *widget = layout->Display();
        Drag *drag = widget->drag();
        if (drag)
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Arrow modified"))
            {
                Point3 p0 = drag->Origin();
                updateArg(widget, ax,
                          swd*(x-p0.x)+sw*w/2, swd*(x-p1.x)+sw*w/2,
                          swd*(x-p2.x)+sw*w/2,
                          0.0, sw*w/df);
                if (h != 0)
                {
                    updateArg(widget, ary,
                              2*sh*(p0.y-y)/h,
                              2*sh*(p1.y-y)/h,
                              2*sh*(p2.y-y)/h,
                              0.0, 1.0);
                }
                changed = true;
            }
        }
    }
    return changed;
}



// ============================================================================
//
//   A polygon manipulator udpates x, y, w, h, and the number of points
//
// ============================================================================

ControlPolygon::ControlPolygon(Tree *self,
                               Real *x, Real *y, Real *w, Real *h,
                               Integer *p)
// ----------------------------------------------------------------------------
//   A control star adds the number of points to the control rectangle
// ----------------------------------------------------------------------------
    : ControlRectangle(self, x, y, w, h), p(p)
{}


bool ControlPolygon::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for a polygon
// ----------------------------------------------------------------------------
{
    bool changed = ControlRectangle::DrawHandles(layout);

    int sw = w > 0? 1: -1;
    int sh = h > 0? 1: -1;
    int p_min = 3;
    int p_max = 20;
    int pp = p;
    if (p < p_min)
        pp = p_min;
    if (p > p_max)
        pp = p_max;

    Point3 handle = Point3(x-sw*w/2+sw*w*(pp-2)/19, y-sh*h/2, 0);
    if (!IsMarkedConstant(p) &&
        DrawHandle(layout, handle, 9, "adjust_shape_handle"))
    {
        Widget *widget = layout->Display();
        Drag *drag = widget->drag();
        if (drag)
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 &&
                widget->markChange("Number of points changed"))
            {
                Point3 p0 = drag->Origin();
                coord p0x = 19*sw*(p0.x - x)/w + 11.5;
                coord p1x = 19*sw*(p1.x - x)/w + 11.5;
                coord p2x = 19*sw*(p2.x - x)/w + 11.5;
                updateArg(widget, p, p0x, p1x, p2x, p_min, p_max);
                changed = true;
            }
        }
    }
    return changed;
}



// ============================================================================
//
//   A star manipulator udpates x, y, w, h, number of points and inner radius
//
// ============================================================================

ControlStar::ControlStar(Tree *self,
                         Real *x, Real *y, Real *w, Real *h,
                         Integer *p, Real *r)
// ----------------------------------------------------------------------------
//   A control star adds inner circle ratio to the control polygon
// ----------------------------------------------------------------------------
    : ControlPolygon(self, x, y, w, h, p), r(r)
{}


bool ControlStar::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for a star
// ----------------------------------------------------------------------------
{
    bool changed = false;

    double cp = cos(M_PI/p);
    double sp = sin(M_PI/p);
    int sw = w > 0? 1: -1;
    int sh = h > 0? 1: -1;
    int r_min = 0.0;
    int r_max = 1.0;
    double rr = r;
    if (r < r_min)
        rr = r_min;
    if (r > r_max)
        rr = r_max;

    Point3 handle = Point3(x + rr*sw*w/2*sp, y + rr*h/2*cp, 0);
    if (!IsMarkedConstant(r) &&
        DrawHandle(layout, handle, 11, "adjust_shape_handle"))
    {
        Widget *widget = layout->Display();
        Drag *drag = widget->drag();
        if (drag)
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Star inner circle changed"))
            {
                Point3 p0 = drag->Origin();
                scale hp = sqrt(w*sp*w*sp + h*cp*h*cp)*sh*cp/2;
                updateArg(widget, r,
                          (p0.y - y)/hp, (p1.y - y)/hp, (p2.y - y)/hp,
                          r_min, r_max);
                changed = true;
            }
        }
    }
    if (!changed)
    {
        changed = ControlPolygon::DrawHandles(layout);
    }
    return changed;
}



// ============================================================================
//
//   A ballon manipulator udpates x, y, w, h, radius r and end of tail a
//
// ============================================================================

ControlBalloon::ControlBalloon(Tree *self,
                               Real *x, Real *y, Real *w, Real *h,
                               Real *r, Real *ax, Real *ay)
// ----------------------------------------------------------------------------
//   A control balloon adds a tail to the control rounded rectangle
// ----------------------------------------------------------------------------
    : ControlRoundedRectangle(self, x, y, w, h, r), ax(ax), ay(ay)
{}


bool ControlBalloon::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for a balloon
// ----------------------------------------------------------------------------
{
    bool changed = ControlRoundedRectangle::DrawHandles(layout);

    Point3 handle = Point3(ax, ay, 0);
    if (!(IsMarkedConstant(ax) && IsMarkedConstant(ay)) &&
        DrawHandle(layout, handle, 11, "adjust_shape_handle"))
    {
        Widget *widget = layout->Display();
        Drag *drag = widget->drag();
        if (drag)
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Balloon tail changed"))
            {
                Point3 p0 = drag->Origin();
                updateArg(widget, ax, p0.x, p1.x, p2.x);
                updateArg(widget, ay, p0.y, p1.y, p2.y);
                changed = true;
            }
        }
    }
    return changed;
}



// ============================================================================
//
//   A callout manipulator udpates x, y, w, h, radius r, end and width of tail
//
// ============================================================================

ControlCallout::ControlCallout(Tree *self,
                               Real *x, Real *y, Real *w, Real *h,
                               Real *r, Real *ax, Real *ay, Real *d)
// ----------------------------------------------------------------------------
//   A control callout adds a width to the tail to the control balloon
// ----------------------------------------------------------------------------
    : ControlBalloon(self, x, y, w, h, r, ax, ay), d(d)
{}


bool ControlCallout::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for a balloon
// ----------------------------------------------------------------------------
{
    bool changed = ControlBalloon::DrawHandles(layout);

    int sw = w > 0? 1: -1;
    int sh = h > 0? 1: -1;
    double pw = sw*w;
    double ph = sh*h;
    double rr = r;
    if (pw < ph)
    {
        if (r > pw/2)
            rr = pw/2;
    }
    else
    {
        if (r > ph/2)
            rr = ph/2;
    }
    double dd = d;
    if (dd < 0)
        dd = 0;
    dd = dd < pw? dd: double(pw);
    dd = dd < ph? dd: double(ph);
    double mrd = dd > 2*rr? dd: 2*rr;
    double tx = double(ax) - double(x);
    double ty = double(ay) - double(y);
    int stx = tx > 0? 1: -1;
    int sty = ty > 0? 1: -1;
    double ptx = stx*tx;
    double pty = sty*ty;

    Point cd;
    if (pty <= (ph-mrd)/2)
    {
        // Horizontal tail
        cd.x = x + stx*(pw-mrd)/2;
        cd.y = y + sty*(pty < (ph-mrd)/2? pty: (ph-mrd)/2);
    }
    else if (ptx <= (pw-mrd)/2)
    {
        // Vertical tail
        cd.x = x + stx*(ptx < (pw-mrd)/2? ptx: (pw-mrd)/2);
        cd.y = y + sty*(ph-mrd)/2;
    }
    else
    {
        // Tail with an angle
        cd.x = x + stx*(pw-mrd)/2;
        cd.y = y + sty*(ph-mrd)/2;
    }

    Vector td = Point(ax, ay) - cd;
    td.Normalize();
    double beta = sty*acos(td.x);

    Point dcp;
    dcp.x = ax + dd/4*cos(beta+M_PI_2) - 15*td.x;
    dcp.y = ay + dd/4*sin(beta+M_PI_2) - 15*td.y;

    Point3 handle = Point3(dcp.x, dcp.y, 0);
    if (!IsMarkedConstant(d) &&
        DrawHandle(layout, handle, 13, "adjust_shape_handle"))
    {
        Widget *widget = layout->Display();
        Drag *drag = widget->drag();
        if (drag)
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Callout tail width changed"))
            {
                Point3 p0 = drag->Origin();
                if (-sty*cos(beta+M_PI_2) > stx*sin(beta+M_PI_2))
                {
                    updateArg(widget, d, -sty*4*p0.x, -sty*4*p1.x, -sty*4*p2.x,
                              0, pw<ph? pw: ph );
                }
                else
                {
                    updateArg(widget, d, stx*4*p0.y, stx*4*p1.y, stx*4*p2.y,
                              0, pw<ph? pw: ph );
                }
                changed = true;
            }
        }
    }
    return changed;
}



// ============================================================================
//
//   Manipulate an image
//
// ============================================================================

ImageManipulator::ImageManipulator(Tree *self,
                                   Real *x, Real *y, Real *sx, Real *sy,
                                   double w, double h)
// ----------------------------------------------------------------------------
//    Create a widget manipulator within the given rectangle
// ----------------------------------------------------------------------------
    : ControlRectangle(self, x,y, sx, sy), w0(w), h0(h)
{
    if (w0 == 0.0) w0 = 1.0;
    if (h0 == 0.0) h0 = 1.0;
}



// ============================================================================
//
//   Manipulate a widget
//
// ============================================================================

WidgetManipulator::WidgetManipulator(Tree *self,
                                     Real *x, Real *y, Real *w, Real *h,
                                     WidgetSurface *s)
// ----------------------------------------------------------------------------
//    Create a widget manipulator within the given rectangle
// ----------------------------------------------------------------------------
    : FrameManipulator(self, x, y, w, h), surface(s)
{}


void WidgetManipulator::DrawSelection(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the selection as usual, and if selected, request focus
// ----------------------------------------------------------------------------
{
    Widget *widget = layout->Display();
    bool selected = widget->selected(layout);
    Manipulator::DrawSelection(layout);
    if (selected)
    {
        if (surface->requestFocus(layout, x, y))
        {
            Box3 bounds = Bounds(layout);
            XL::Save<Point3> zeroOffset(layout->offset, Point3(0,0,0));
            widget->drawSelection(layout, bounds,
                                  "widget_selection", layout->id);
        }
    }
}



// ============================================================================
//
//   Manipulate a table
//
// ============================================================================

TableManipulator::TableManipulator(Tree *self,
                                   Real *x, Real *y, Table *table)
// ----------------------------------------------------------------------------
//    Create a table manipulator at the current coordinates
// ----------------------------------------------------------------------------
    : FrameManipulator(self, x, y, new Real(200), new Real(100)), table(table)
{}


bool TableManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Allow a table to be moved around
// ----------------------------------------------------------------------------
{
    Box3 bounds = table->Bounds(layout);
    w = new Real(bounds.Width());
    h = new Real(bounds.Height());
    return FrameManipulator::DrawHandles(layout);
}



// ============================================================================
//
//   A GraphicPathManipulator is able to update all the points in a path
//
// ============================================================================

GraphicPathManipulator::GraphicPathManipulator(Tree_p self,
                                               GraphicPath * path,
                                               Tree_p path_tree)
// ----------------------------------------------------------------------------
//   A path manipulator owns a given child and manipulates it
// ----------------------------------------------------------------------------
    : FrameManipulator(self,
                       new XL::Real(), new XL::Real(),  // x, y
                       new XL::Real(), new XL::Real()), // w, h
      path(path), path_tree(path_tree)
{}


bool GraphicPathManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles around a graphic path
// ----------------------------------------------------------------------------
//   REVISIT: currently the bounding box includes the exterior control points,
//   we probably want to limit the bb to the actual drawing
{
    // Shape manipulations are first handled at the bounding box level, then
    // individual path control points are moved proportionally
    Widget *widget = layout->Display();
    uint sel = widget->selected(layout);
    if ((sel & Widget::SELECTION_MASK) != Widget::CONTAINER_OPENED)
        return false;

    GraphicPathInfo *path_info = path_tree->GetInfo<GraphicPathInfo>();
    if (!path_info)
    {
        // Save info about the initial state of the graphic path
        path_info = new GraphicPathInfo(path);
        path_tree->SetInfo<GraphicPathInfo> (path_info);
    }

    // Set manipulator dimensions = current bounding box
    Box3 b1 = path->bounds;
    x->value = b1.Center().x;
    y->value = b1.Center().y;
    w->value = b1.Width();
    h->value = b1.Height();

    // Display handles and deal with resize/rotate through FrameManipulator
    bool changed = FrameManipulator::DrawHandles(layout);

    // Handle shape move
    if (!changed)
    {
        Drag   *drag = widget->drag();
        if (drag && (layout->groupDrag || !widget->selectionHandleId()))
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Path moved"))
            {
                Point3 p0 = drag->Origin();
                updateArg(widget, x, p0.x, p1.x, p2.x);
                updateArg(widget, y, p0.y, p1.y, p2.y);
                changed = true;
            }
        }
    }

    if (changed)
    {
        // Update each control point to reflect new shape bounding box
        Box3 b0 = path_info->b0;
        Point3 lower(x - w/2, y - h/2, 0);
        Point3 upper(x + w/2, y + h/2, 0);
        Box3 b2(lower, upper);

        GraphicPath::control_points::iterator i;
        std::vector<Point3>::iterator j;
        for (i = path->controls.begin(), j = path_info->controls.begin();
             i != path->controls.end();
             i++, j++)
        {
            Point3 cp0 = *j;
            scale kx = (cp0.x - b0.Left())   / b0.Width();
            scale ky = (cp0.y - b0.Bottom()) / b0.Height();

            Vector3 v1 (kx * b1.Width(), ky * b1.Height(), 0);
            Point3 cp1 = b1.lower + v1;

            Vector3 v2 (kx * b2.Width(), ky * b2.Height(), 0);
            Point3 cp2 = b2.lower + v2;

            ControlPoint *child = *i;
            updateArg(widget, child->x, cp0.x, cp1.x, cp2.x);
            updateArg(widget, child->y, cp0.y, cp1.y, cp2.y);
        }
    }

    return changed;
}



// ============================================================================
//
//   A box manipulator displays the 8 external control points
//
// ============================================================================

BoxManipulator::BoxManipulator(Tree *self,
                               Real *x, Real *y, Real *z,
                               Real *w, Real *h, Real *d)
// ----------------------------------------------------------------------------
//   A control rectangle owns a given child and manipulates it
// ----------------------------------------------------------------------------
    : Manipulator(self), x(x), y(y), z(z), w(w), h(h), d(d)
{}


bool BoxManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles around a widget
// ----------------------------------------------------------------------------
{
    Widget *widget = layout->Display();
    coord   xx = x, yy = y, zz = z, ww = w, hh = h, dd = d;
    Drag   *drag = widget->drag();
    uint    handle = 0;

    for (uint hn = 0; hn < 8; hn++)
    {
        short  sw = (hn & 1) ? 1 : -1;
        short  sh = (hn & 2) ? 1 : -1;
        short  sd = (hn & 4) ? 1 : -1;

        // Lower-left corner
        if (DrawHandle(layout, Point3(xx+sw*ww/2,yy+sh*hh/2,zz+sd*dd/2), hn+1))
        {
            if (!handle)
            {
                handle = hn+1;

                // Update arguments if necessary
                if (drag)
                {
                    Point3 p1 = drag->Previous();
                    Point3 p2 = drag->Current();
                    text   t1 = sh < 0 ? "Lower " : "Upper ";
                    text   t2 = sw < 0 ? "left " : "right ";
                    text   t3 = sd < 0 ? "front " : "back ";
                    if (p1 != p2 &&
                        widget->markChange(t1 + t2 + t3 + "corner moved"))
                    {
                        Point3 p0 = drag->Origin();
                        if (hn < 4)
                        {
                            updateArg(widget, x, p0.x/2, p1.x/2, p2.x/2);
                            updateArg(widget, w, sw*p0.x, sw*p1.x, sw*p2.x);
                        }
                        else
                        {
                            updateArg(widget, z, p0.x/2, p1.x/2, p2.x/2);
                            updateArg(widget, d, sd*p0.x, sd*p1.x, sd*p2.x);
                        }
                        updateArg(widget, y, p0.y/2, p1.y/2, p2.y/2);
                        updateArg(widget, h, sh*p0.y, sh*p1.y, sh*p2.y);
                    }
                }
            }
        }
    }

    return handle != 0;
}



// ============================================================================
//
//   A control box manipulator udpates 3D position and size
//
// ============================================================================

ControlBox::ControlBox(Tree *self,
                       Real *x, Real *y, Real *z,
                       Real *w, Real *h, Real *d)
// ----------------------------------------------------------------------------
//   A control rectangle owns a given child and manipulates it
// ----------------------------------------------------------------------------
    : BoxManipulator(self, x, y, z, w, h, d)
{}


bool ControlBox::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the handles for a 3D object
// ----------------------------------------------------------------------------
{
    // Check if we clicked anywhere else in the shape
    bool changed = BoxManipulator::DrawHandles(layout);
    if (!changed)
    {
        Widget *widget = layout->Display();
        Drag *drag = widget->drag();
        if (drag && !widget->selectionHandleId())
        {
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("3D shape moved"))
            {
                Point3 p0 = drag->Origin();
                updateArg(widget, x, p0.x, p1.x, p2.x);
                updateArg(widget, y, p0.y, p1.y, p2.y);
                updateArg(widget, z, p0.z, p1.z, p2.z);
                changed = true;
            }
        }
    }
    return changed;
}



// ============================================================================
//
//   A TransformManipulator is used for rotation, translation, scale
//
// ============================================================================

TransformManipulator::TransformManipulator(Tree *self)
// ----------------------------------------------------------------------------
//   Record the child we own
// ----------------------------------------------------------------------------
    : Manipulator(self)
{}



// ============================================================================
//
//   Rotation manipulator
//
// ============================================================================

RotationManipulator::RotationManipulator(Tree *self,
                                         Real *a, Real *x,Real *y,Real *z)
// ----------------------------------------------------------------------------
//   Manipulation of a rotation
// ----------------------------------------------------------------------------
    : TransformManipulator(self), a(a), x(x), y(y), z(z)
{}


void RotationManipulator::Identify(Layout *layout)
// ----------------------------------------------------------------------------
//   Remember the last rotation for subsequent shapes
// ----------------------------------------------------------------------------
{
    TransformManipulator::Identify(layout);
}


bool RotationManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Draw the selection for a rotation
// ----------------------------------------------------------------------------
{
    Widget  *widget  = layout->Display();
    Vector3  offset  = layout->Offset();
    Drag    *drag    = widget->drag();
    bool     changed = false;

    // Vector around which we rotate (normalized)
    Vector3 tip = Vector3(x, y, z);
    tip.Normalize();
    if (DrawHandle(layout, 50*tip + offset, 0x1001, "rotation_tip"))
    {
        if (drag)
        {
            // Compute the desired tip position
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Changed rotation direction"))
            {
                Point3 p0 = drag->Origin();
                updateArg(widget, x, p0.x, p1.x, p2.x);
                updateArg(widget, y, p0.y, p1.y, p2.y);
                updateArg(widget, z, p0.z, p1.z, p2.z);
                changed = true;
            }
        }
    }

    // Draw the sphere
    if (DrawHandle(layout, offset, 0x1002, "rotation_base"))
    {
        if (drag && !changed)
        {
            // Compute the desired angle
            Point3 p1 = drag->Previous();
            Point3 p2 = drag->Current();
            if (p1 != p2 && widget->markChange("Changed rotation amount"))
            {
                Point3 p0 = drag->Origin();

                Vector3 v0 = Vector3(p0) - offset;
                double  a0 = atan2(v0.y, v0.x) * (180 / M_PI);
                Vector3 v1 = Vector3(p1) - offset;
                double  a1 = atan2(v1.y, v1.x) * (180 / M_PI);
                Vector3 v2 = Vector3(p2) - offset;
                double  a2 = atan2(v2.y, v2.x) * (180 / M_PI);

                updateArg(widget, a,  a0, a1, a2);
                updateArg(widget, a,  a0, a1, a2);
                changed = true;
            }
        }
    }

    return changed;
}



// ============================================================================
//
//   Translation manipulator
//
// ============================================================================

TranslationManipulator::TranslationManipulator(Tree *self,
                                               Real *x, Real *y, Real *z)
// ----------------------------------------------------------------------------
//   Manipulation of a translation
// ----------------------------------------------------------------------------
    : TransformManipulator(self), x(x), y(y), z(z)
{}


void TranslationManipulator::Identify(Layout *layout)
// ----------------------------------------------------------------------------
//   Remember the last translation for subsequent shapes
// ----------------------------------------------------------------------------
{
    TransformManipulator::Identify(layout);
}


bool TranslationManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Manipulate the translation
// ----------------------------------------------------------------------------
{
    Widget  *widget = layout->Display();
    Vector3  offset = layout->Offset();
    Drag    *drag   = widget->drag();
    uint     handle = 0;

    // Draw the translation base
    if (DrawHandle(layout, offset, 0x2001, "translation_base"))
        handle = 1;

    // Size of the X, Y, Z direction
    if (DrawHandle(layout, offset, 0x2002, "translation_x"))
        handle = 2;

    if (DrawHandle(layout, offset, 0x2003, "translation_y"))
        handle = 3;

    if (DrawHandle(layout, offset, 0x2004, "translation_z"))
        handle = 4;

    if (handle && drag)
    {
        Point3 p1 = drag->Previous();
        Point3 p2 = drag->Current();
        if (p1 != p2)
        {
            Point3 p0 = drag->Origin();

            switch(handle)
            {
            case 1:
                if (widget->markChange("Updated translation"))
                {
                    updateArg(widget, x,  p0.x, p1.x, p2.x);
                    updateArg(widget, y,  p0.y, p1.y, p2.y);
                }
                break;
            case 2:
                if (widget->markChange("Updated translation X axis"))
                    updateArg(widget, x,  p0.x, p1.x, p2.x);
                break;
            case 3:
                if (widget->markChange("Updated translation Y axis"))
                    updateArg(widget, y,  p0.y, p1.y, p2.y);
                break;
            case 4:
                if (widget->markChange("Updated translation Z axis"))
                    updateArg(widget, z,  p0.x-p0.y, p1.x-p1.y, p2.x-p2.y);
                break;
            }

            return true;
        }
    }

    return false;
};




// ============================================================================
//
//   Scale manipulator
//
// ============================================================================

ScaleManipulator::ScaleManipulator(Tree *self, Real *x, Real *y, Real *z)
// ----------------------------------------------------------------------------
//   Manipulation of a scale
// ----------------------------------------------------------------------------
    : TransformManipulator(self), x(x), y(y), z(z)
{}


void ScaleManipulator::Identify(Layout *layout)
// ----------------------------------------------------------------------------
//   Remember the last scaling for subsequent shapes
// ----------------------------------------------------------------------------
{
    TransformManipulator::Identify(layout);
}


bool ScaleManipulator::DrawHandles(Layout *layout)
// ----------------------------------------------------------------------------
//   Manipulate the scale
// ----------------------------------------------------------------------------
{
    Widget  *widget = layout->Display();
    Vector3  offset = layout->Offset();
    Drag    *drag   = widget->drag();
    uint     handle = 0;
    coord    s = 0.01;

    // Draw the scale base
    if (DrawHandle(layout, offset, 0x4001, "scale_base"))
        handle = 1;

    // Size of the X, Y, Z direction
    if (DrawHandle(layout, offset, 0x4002, "scale_x"))
        handle = 2;

    if (DrawHandle(layout, offset, 0x4003, "scale_y"))
        handle = 3;

    if (DrawHandle(layout, offset, 0x4004, "scale_z"))
        handle = 4;

    if (handle && drag)
    {
        Point3 p1 = drag->Previous();
        Point3 p2 = drag->Current();
        if (p1 != p2)
        {
            Point3 p0 = drag->Origin();
            coord  v0 = s * (p0.x + p0.y + p0.z);
            coord  v1 = s * (p1.x + p1.y + p1.z);
            coord  v2 = s * (p2.x + p2.y + p2.z);

            switch(handle)
            {
            case 1:
                if (widget->markChange("Updated scale"))
                {
                    updateArg(widget, x, v0, v1, v2);
                    updateArg(widget, y, v0, v1, v2);
                    updateArg(widget, z, v0, v1, v2);
                }
                break;
            case 2:
                if (widget->markChange("Updated scale X axis"))
                    updateArg(widget, x,  s * p0.x, s * p1.x, s * p2.x);
                break;
            case 3:
                if (widget->markChange("Updated scale Y axis"))
                    updateArg(widget, y,  s * p0.y, s * p1.y, s * p2.y);
                break;
            case 4:
                if (widget->markChange("Updated scale Z axis"))
                    updateArg(widget, z,  v0, v1, v2);
                break;
            }

            return true;
        }
    }

    return false;
};

TAO_END



// ****************************************************************************
// 
//    Code generation from manipulators.tbl
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
#include "manipulator.tbl"

namespace Tao
{

#include "manipulator.tbl"


void EnterManipulators()
// ----------------------------------------------------------------------------
//   Enter all the basic operations defined in attributes.tbl
// ----------------------------------------------------------------------------
{
    XL::Context *context = MAIN->context;
#include "opcodes_define.h"
#include "manipulator.tbl"
}


void DeleteManipulators()
// ----------------------------------------------------------------------------
//   Delete all the global operations defined in attributes.tbl
// ----------------------------------------------------------------------------
{
#include "opcodes_delete.h"
#include "manipulator.tbl"
}

}
