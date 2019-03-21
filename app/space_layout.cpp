// ****************************************************************************
//  space_layout.cpp                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//     Layout objects in 3D space (z-ordering, ...)
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

#include "space_layout.h"
#include "attributes.h"

TAO_BEGIN

SpaceLayout::SpaceLayout(Widget *widget)
// ----------------------------------------------------------------------------
//   Constructor sets defaults
// ----------------------------------------------------------------------------
    : Layout(widget)
{
    IFTRACE(justify)
            std::cerr << "<->SpaceLayout::SpaceLayout "<<this << std::endl;

}


SpaceLayout::~SpaceLayout()
// ----------------------------------------------------------------------------
//   Destructor
// ----------------------------------------------------------------------------
{
    IFTRACE(justify)
        std::cerr << "<->SpaceLayout::~SpaceLayout "<<this << std::endl;
}


Box3 SpaceLayout::Space(Layout *layout)
// ----------------------------------------------------------------------------
//   Return the space for the layout
// ----------------------------------------------------------------------------
{
    Box3 result = Bounds(layout);
    result |= space;
    return result;
}

TAO_END
