#ifndef COLOR_H
#define COLOR_H
// ****************************************************************************
//  color.h                                                         Tao project
// ****************************************************************************
//
//   File Description:
//
//    Representation of colors (essentially as 4-dimensional vectors)
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

#include "tao.h"

TAO_BEGIN

struct Color
// ----------------------------------------------------------------------------
//    A representation for RGBA color
// ----------------------------------------------------------------------------
{
    typedef float percent;

    Color(percent r = 0.0, percent g = 0.0, percent b = 0.0, percent a = 0.0)
        : red(r), green(g), blue(b), alpha(a) {}
    Color(const Color &o)
        : red(o.red), green(o.green), blue(o.blue), alpha(o.alpha) {}

    Color& operator =(const Color& o)
    {
        red   = o.red;
        blue  = o.blue;
        green = o.green;
        alpha = o.alpha;
        return *this;
    }

    bool operator ==(const Color& o)
    {
        return (red   == o.red    &&
                blue  == o.blue  &&
                green == o.green &&
                alpha == o.alpha);
    }

    bool operator !=(const Color& o)
    {
        return !operator==(o);
    }

    Color& Set(percent r, percent g, percent b, percent a = 1)
    {
        red   = r;
        blue  = b;
        green = g;
        alpha = a;
        return *this;
    }

    Color& operator +=(const Color& o)
    {
        red   += o.red;
        blue  += o.blue;
        green += o.green;
        alpha += o.alpha;
        return *this;
    }

    Color& operator -=(const Color& o)
    {
        red   -= o.red;
        blue  -= o.blue;
        green -= o.green;
        alpha -= o.alpha;
        return *this;
    }

    Color& operator *=(scale s)
    {
        red   *= s;
        blue  *= s;
        green *= s;
        alpha *= s;
        return *this;
    }

    Color& operator *=(const Color& c)
    {
        red   *= c.red;
        blue  *= c.blue;
        green *= c.green;
        alpha *= c.alpha;
        return *this;
    }

    Color& operator /=(scale s)
    {
        red   /= s;
        blue  /= s;
        green /= s;
        alpha /= s;
        return *this;
    }

    Color& operator /=(const Color& c)
    {
        red   /= c.red;
        blue  /= c.blue;
        green /= c.green;
        alpha /= c.alpha;
        return *this;
    }

    scale Intensity() const
    {
        return sqrt(red*red+green*green+blue*blue);
    }

    Color& Normalize()
    {
        if (red < 0)    red = 0;
        if (green < 0)  green = 0;
        if (blue < 0)   blue = 0;
        if (alpha < 0)  alpha = 0;
        if (red > 1)    red = 1;
        if (green > 1)  green = 1;
        if (blue > 1)   blue = 1;
        if (alpha > 1)  alpha = 1;
        return *this;
    }

    percent *Data()     { return &red; }

public:
    percent red, green, blue, alpha;
};



// ============================================================================
//
//   Inline Color operations not defined in class
//
// ============================================================================

inline Color operator +(const Color& l, const Color &r)
{
    Color result(l);
    result += r;
    return result;
}

inline Color operator -(const Color& l, const Color &r)
{
    Color result(l);
    result -= r;
    return result;
}

inline Color operator *(const Color& l, scale s)
{
    Color result(l);
    result *= s;
    return result;
}

inline Color operator *(scale s, const Color& l)
{
    Color result(l);
    result *= s;
    return result;
}

inline Color operator *(const Color& c, const Color& l)
{
    Color result(l);
    result *= c;
    return result;
}

inline Color operator /(const Color& c, const Color& l)
{
    Color result(l);
    result /= c;
    return result;
}

inline std::ostream& operator <<(std::ostream &out, const Color &color)
{
    out << "Color (" << color.red << ", " << color.green << ", "
            << color.blue << ", " << color.alpha << ") ";
    return out;
}

TAO_END

#endif // COLOR_H
