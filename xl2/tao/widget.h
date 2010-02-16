#ifndef WIDGET_H
#define WIDGET_H
// ****************************************************************************
//  widget.h                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//    The primary graphical widget used to display Tao contents
//
//
//
//
//
//
//
//
// ****************************************************************************
// This document is released under the GNU General Public License.
// See http://www.gnu.org/copyleft/gpl.html and Matthew 25:22 for details
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Lionel Schaffhauser <lionel@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include <QtOpenGL>
#include <QImage>
#include <QTimeLine>
#include <QSvgRenderer>
#include "main.h"
#include "tao.h"

namespace Tao {

class Widget : public QGLWidget
// ----------------------------------------------------------------------------
//   This is the widget we use to display XL programs output
// ----------------------------------------------------------------------------
{
    Q_OBJECT
public:
    Widget(QWidget *parent, XL::SourceFile *sf = NULL);
    ~Widget();

    // Events
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void timerEvent(QTimerEvent *);
    void wheelEvent(QWheelEvent *);
    
public:
    typedef XL::Tree Tree;

    // XLR entry points
    static Widget *Tao() { return current; }
    Tree *caption(Tree *self, text t);
    Tree *drawSvg(Tree *self, text t);

    Tree *rotateX(Tree *self, double rx);
    Tree *rotateY(Tree *self, double ry);
    Tree *rotateZ(Tree *self, double rz);
    Tree *rotate(Tree *self, double rx, double ry, double rz, double ra);

    Tree *translateX(Tree *self, double x);
    Tree *translateY(Tree *self, double y);
    Tree *translateZ(Tree *self, double z);
    Tree *translate(Tree *self, double x, double y, double z);

    Tree *scaleX(Tree *self, double x);
    Tree *scaleY(Tree *self, double y);
    Tree *scaleZ(Tree *self, double z);
    Tree *scale(Tree *self, double x, double y, double z);

    Tree *locally(Tree *self, Tree *t);

    Tree *color(Tree *self, double r, double g, double b, double a);

    Tree *refresh(Tree *self, double delay);
    Tree *time(Tree *self);

    Tree *polygon(Tree *self, Tree *t);
    Tree *vertex(Tree *self, double x, double y, double z);
    Tree *sphere(Tree *self, double x, double y, double z, double r);

    Tree *texture(Tree *self, text n, Tree *body);
    Tree *texCoord(Tree *self, double x, double y);

public slots:
    void draw();

public:
    // XL Runtime
    XL::SourceFile *  xlProgram;
    text              caption_text;
    static Widget    *current;
};


#define TAO(x)  (Tao::Widget::Tao() ? Tao::Widget::Tao()->x : 0)
#define RTAO(x) return TAO(x)

} // namespace Tao

#endif // TAO_WIDGET_H
