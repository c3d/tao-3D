#ifndef TRANSFORMS_H
#define TRANSFORMS_H
// ****************************************************************************
//  transforms.h                                                    Tao project
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

#include "attributes.h"

TAO_BEGIN

struct Transform : Attribute
// ----------------------------------------------------------------------------
//   Base class for all transforms
// ----------------------------------------------------------------------------
{
    Transform() : Attribute() {}
    virtual void Draw(Layout *where);
    virtual void DrawSelection(Layout *where);
    virtual void Identify(Layout *where);
};


struct ResetTransform : Transform
// ----------------------------------------------------------------------------
//   Reset transform to what it was initially
// ----------------------------------------------------------------------------
{
    ResetTransform() {}
    virtual void Draw(Layout *where);
};


struct Rotation : Transform
// ----------------------------------------------------------------------------
//    Record a rotation
// ----------------------------------------------------------------------------
{
    Rotation(coord a, coord x, coord y, coord z):
        Transform(), amount(a), xaxis(x), yaxis(y), zaxis(z) {}
    virtual void Draw(Layout *where);
    coord amount, xaxis, yaxis, zaxis;
};


struct Translation : Transform
// ----------------------------------------------------------------------------
//    Record a translation
// ----------------------------------------------------------------------------
{
    Translation(coord x, coord y, coord z):
        Transform(), xaxis(x), yaxis(y), zaxis(z) {}
    virtual void Draw(Layout *where);
    coord xaxis, yaxis, zaxis;
};


struct Scale : Transform
// ----------------------------------------------------------------------------
//    Record a scale change
// ----------------------------------------------------------------------------
{
    Scale(scale x, scale y, scale z):
        Transform(), xaxis(x), yaxis(y), zaxis(z) {}
    virtual void Draw(Layout *where);
    scale xaxis, yaxis, zaxis;
};


struct ClipPlane : Transform
// ----------------------------------------------------------------------------
//    Record a clip plane change
// ----------------------------------------------------------------------------
{
    ClipPlane(int plane, scale a, scale b, scale c, scale d):
        Transform(), plane(plane), a(a), b(b), c(c), d(d) {}
    virtual void Draw(Layout *where);
    virtual void Evaluate(Layout *) {}
    int plane;
    scale a, b, c, d;
};


struct MoveTo : Transform
// ----------------------------------------------------------------------------
//    Record a move-to operation
// ----------------------------------------------------------------------------
{
    MoveTo(coord x, coord y, coord z):
        Transform(), xaxis(x), yaxis(y), zaxis(z) {}
    virtual void Draw(Layout *where);
    scale xaxis, yaxis, zaxis;
};


struct MoveToRel : MoveTo
// ----------------------------------------------------------------------------
//    Record a move-to operation
// ----------------------------------------------------------------------------
{
    MoveToRel(coord x, coord y, coord z): MoveTo(x, y, z) {}
    virtual void Draw(Layout *where);
};


void printMatrix(GLint model = -1);
// ----------------------------------------------------------------------------
//    Print GL matrix on stderr
// ----------------------------------------------------------------------------



// ============================================================================
// 
//   Entering transforms in the symbols table
// 
// ============================================================================

extern void EnterTransforms();
extern void DeleteTransforms();

TAO_END

#endif // TRANSFORMS_H
