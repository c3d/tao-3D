#ifndef WIDGET_SURFACE_H
#define WIDGET_SURFACE_H
// ****************************************************************************
//  widget_surface.h                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//    Encapsulate a web view that can be rendered as a texture
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
//  (C) 2010 Catherine Burvelle <cathy@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "tao.h"
#include "tao_tree.h"
#include "coords.h"
#include "tao_gl.h"
#include "info_trash_can.h"
#include "tao/tao_info.h"
#include <QtGui>
#include <QAbstractButton>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QAbstractSlider>

namespace Tao {

struct Widget;
struct Layout;


struct WidgetSurface : QObject, Tao::Info, InfoTrashCan
// ----------------------------------------------------------------------------
//    Hold information about the the widget associated to a given tree
// ----------------------------------------------------------------------------
{
    Q_OBJECT
public:
    typedef WidgetSurface * data_t;

    WidgetSurface(Tree *t, QWidget *widget, Widget *display);
    virtual ~WidgetSurface();
    virtual void Delete() { trash.push_back(this); }

    operator            data_t() { return this; }
    void                resize(uint width, uint height);
    virtual GLuint      bind();
    virtual bool        requestFocus(Layout *l, coord x, coord y);
    int                 width()         { return widget->width(); }
    int                 height()        { return widget->height(); }
    Tree *              evaluate(Tree * t);
    Widget *            display()       { return parentDisplay; }

public:
    QWidget *           widget;
    Widget *            parentDisplay;
    GLuint              textureId;
    bool                dirty;
    XL::Tree_p          tree;


protected slots:
    void                repaint();
};


#ifndef CFG_NO_QTWEBKIT
struct WebViewSurface : WidgetSurface
// ----------------------------------------------------------------------------
//    Hold information about a QWebView
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    typedef WebViewSurface * data_t;
    WebViewSurface(XL::Tree *t, Widget *parent);
    operator data_t() { return this; }
    virtual GLuint bindURL(XL::Text *url, XL::Integer_p progress=NULL);

private:
    XL::Text_p    url;
    XL::Integer_p progress;
    text          inputUrl, currentUrl;

protected slots:
    void        finishedLoading(bool loadedOK);
    void        loadProgress(int progress);
};
#endif


struct LineEditSurface : WidgetSurface
// ----------------------------------------------------------------------------
//    Hold information about a QLineEdit
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    LineEditSurface(XL::Text *t, Widget *parent);
    GLuint bindText(XL::Text *text);

    typedef LineEditSurface * data_t;
    operator data_t() { return this; }
private:
    XL::Text_p contents;

public slots:
    void textChanged(const QString &text);
    void inputValidated();
};


struct TextEditSurface : WidgetSurface
// ----------------------------------------------------------------------------
//    Hold information about a QLineEdit
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    TextEditSurface(XL::Text *html, Widget *parent);
    GLuint bindHTML(XL::Text *html);
    
    typedef TextEditSurface * data_t;
    operator data_t() { return this; }

private:
    XL::Text_p contents;

public slots:
    void textChanged();
};


struct AbstractButtonSurface : WidgetSurface
// ----------------------------------------------------------------------------
//    Hold information about a Button
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
//    typedef AbstractButtonSurface * data_t;
    AbstractButtonSurface(XL::Tree *t, QAbstractButton *button, QString name);
    virtual GLuint bindButton(XL::Text *lbl, XL::Tree *action, XL::Text *sel);
    virtual operator data_t() { return this; }

private:
    text       label;
    XL::Tree_p action;
    XL::Text_p isMarked;

public slots:
    void clicked(bool checked);
    void toggled(bool checked);

};


struct PushButtonSurface : AbstractButtonSurface
// ----------------------------------------------------------------------------
//    Hold information about a QPushButton
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    typedef PushButtonSurface * data_t;
    PushButtonSurface(XL::Tree *t, QWidget *parent, QString name):
        AbstractButtonSurface(t,new QPushButton(parent), name)
    {
        connect((QPushButton*)widget, SIGNAL(clicked(bool)),
                this,                 SLOT(clicked(bool)));
    }
    operator data_t() { return this; }
};


struct RadioButtonSurface : AbstractButtonSurface
// ----------------------------------------------------------------------------
//    Hold information about a QPushButton
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    typedef RadioButtonSurface * data_t;
    RadioButtonSurface(XL::Tree *t, QWidget *parent, QString name):
        AbstractButtonSurface(t, new QRadioButton(parent), name){}
    operator data_t() { return this; }
};


struct CheckBoxSurface : AbstractButtonSurface
// ----------------------------------------------------------------------------
//    Hold information about a QCheckBox button
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    typedef CheckBoxSurface * data_t;
    CheckBoxSurface(XL::Tree *t, QWidget *parent, QString name):
        AbstractButtonSurface(t, new QCheckBox(parent), name){}
    operator data_t() { return this; }
};


struct GroupBoxSurface : WidgetSurface
// ----------------------------------------------------------------------------
//    Hold information about a QGroupBox
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    typedef GroupBoxSurface * data_t;
    GroupBoxSurface(XL::Tree *t, Widget *parent);
    virtual ~GroupBoxSurface();

    operator data_t() { return this; }
    virtual GLuint bindButton(XL::Text *lbl);
    QGridLayout *grid(){
        return (QGridLayout*)widget->layout();
     }


private:
    text label;
};


struct GridGroupBox : QGroupBox
// ----------------------------------------------------------------------------
//    A specialized QGroupBox with a QGridLayout
// ----------------------------------------------------------------------------
{
    GridGroupBox(QWidget* p): QGroupBox(p)
    {
        QGridLayout *grid = new QGridLayout(this);
        grid->setObjectName("groupBox layout");
        setLayout(grid);
    }
protected:
    virtual bool event(QEvent *event);

};


struct AbstractSliderSurface : WidgetSurface
// ----------------------------------------------------------------------------
//    Hold information about a Slider
// ----------------------------------------------------------------------------
{
    Q_OBJECT;
public:
    typedef AbstractSliderSurface * data_t;
    AbstractSliderSurface(XL::Tree *t, QAbstractSlider *parent);
    operator data_t() { return this; }

private:
    int min, max;
    XL::Integer_p value;
public slots:
    void valueChanged(int new_value);

};



// ============================================================================
//
//   Entering widget surfaces in the symbols table
//
// ============================================================================

extern void EnterWidgetSurfaces();
extern void DeleteWidgetSurfaces();

} // namespace Tao

#endif // WIDGET_SURFACE_H
