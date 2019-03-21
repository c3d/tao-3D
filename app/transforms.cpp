// ****************************************************************************
//  transforms.cpp                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//    Record transformations being applied in a layout
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
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "transforms.h"
#include "layout.h"
#include "shapes3d.h"
#include "context.h"
#include "widget.h"


TAO_BEGIN

void Transform::Draw(Layout *where)
// ----------------------------------------------------------------------------
//   Default draws a cube
// ----------------------------------------------------------------------------
{
    Color line(1.0, 0.0, 0.0, 0.5);
    Color fill(0.0, 0.7, 1.0, 0.5);
    XL::Save<Color> saveLine(where->lineColor, line);
    XL::Save<Color> saveFill(where->fillColor, fill);
    Box3 cubeBox(-25, -25, -25, 50, 50, 50);
    Cube cube(cubeBox);
    cube.Draw(where);
}


void Transform::DrawSelection(Layout *where)
// ----------------------------------------------------------------------------
//   Transforms don't have a selection, they just draw themselves
// ----------------------------------------------------------------------------
{
    Draw(where);
}


void Transform::Identify(Layout *where)
// ----------------------------------------------------------------------------
//   Transforms don't have their own identity, they just draw themselves
// ----------------------------------------------------------------------------
{
    Draw(where);
}


void ResetTransform::Draw(Layout *where)
// ----------------------------------------------------------------------------
//   Reset the transformation matrix and other parameters
// ----------------------------------------------------------------------------
{
    Widget *widget = where->Display();
    widget->resetModelviewMatrix();
    GL.HasPixelBlur(false);
}


void Rotation::Draw(Layout *where)
// ----------------------------------------------------------------------------
//    Rotation in a drawing
// ----------------------------------------------------------------------------
{
    // BUG? fmod required to avoid incorrect rotations with large values
    // (>1290000000)
    amount = fmod(amount, 360.0);
    double amod90 = fmod(amount, 90.0);
    if (amod90 < -0.01 || amod90 > 0.01)
        GL.HasPixelBlur(true);
    if (amount != 0.0)
    {
        if (where->offset != Vector3())
        {
            GL.Translate(where->offset.x, where->offset.y, where->offset.z);
            GL.Rotate(amount, xaxis, yaxis, zaxis);
            GL.Translate(-where->offset.x, -where->offset.y, -where->offset.z);
        }
        else
        {
            GL.Rotate(amount, xaxis, yaxis, zaxis);
        }
    }
}


void Translation::Draw(Layout *where)
// ----------------------------------------------------------------------------
//    Rotation in a drawing
// ----------------------------------------------------------------------------
{
    (void) where;
    GL.Translate(xaxis, yaxis, zaxis);
    if (zaxis != 0.0)
        GL.HasPixelBlur(true);
}


void Scale::Draw(Layout *where)
// ----------------------------------------------------------------------------
//    Scale in a drawing
// ----------------------------------------------------------------------------
{
    if (where->offset != Vector3())
    {
        GL.Translate(where->offset.x, where->offset.y, where->offset.z);
        GL.Scale(xaxis, yaxis, zaxis);
        GL.Translate(-where->offset.x, -where->offset.y, -where->offset.z);
    }
    else
    {
        GL.Scale(xaxis, yaxis, zaxis);
    }
    if (xaxis != 1.0 || yaxis != 1.0)
        GL.HasPixelBlur(true);
}


void ClipPlane::Draw(Layout *)
// ----------------------------------------------------------------------------
//   Setup a given clip plane
// ----------------------------------------------------------------------------
{
    static GLint maxClipPlanes = 0;
    if (maxClipPlanes == 0)
        glGetIntegerv(GL_MAX_CLIP_PLANES, &maxClipPlanes);

    if (plane >= 0 && plane < maxClipPlanes && plane <= GL_CLIP_PLANE5)
    {
        GLdouble result[] = { a, b, c, d };
        GL.Enable(GL_CLIP_PLANE0 + plane);
        GL.ClipPlane(GL_CLIP_PLANE0 + plane, result);
    }
}


void MoveTo::Draw(Layout *where)
// ----------------------------------------------------------------------------
//    Position in a drawing
// ----------------------------------------------------------------------------
{
    where->offset = Point3(xaxis, yaxis, zaxis);
}


void MoveToRel::Draw(Layout *where)
// ----------------------------------------------------------------------------
//    Position in a drawing
// ----------------------------------------------------------------------------
{
    where->offset += Vector3(xaxis, yaxis, zaxis);
}


TAO_END



// ****************************************************************************
// 
//    Code generation from transforms.tbl
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
#include "transforms.tbl"

namespace Tao
{

#include "transforms.tbl"


void EnterTransforms()
// ----------------------------------------------------------------------------
//   Enter all the basic operations defined in attributes.tbl
// ----------------------------------------------------------------------------
{
    XL::Context *context = MAIN->context;
#include "opcodes_define.h"
#include "transforms.tbl"
}


void DeleteTransforms()
// ----------------------------------------------------------------------------
//   Delete all the global operations defined in attributes.tbl
// ----------------------------------------------------------------------------
{
#include "opcodes_delete.h"
#include "transforms.tbl"
}

}
