/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains a subclass of the QGraphicsView
 **
 **	Contributors:
 **		Benjamin Tissoires <benjamin.tissoires@enac.fr>
 **
 **
 **	This program is provided to you as free software;
 **	you can redistribute it	and/or modify it under the terms of the
 **	GNU General Public License as published by the Free Software
 **	Foundation; either version 2 of the License, or (at your option)
 **	any later version.
 **/

#include "graphicsview.h"
#include <QDebug>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QMoveEvent>
#include <QMainWindow>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    scene(new QGraphicsScene ()),
    viewRect(QRect ()),
    fullscreen(false),
    maximized(false),
    fitToScreen(false)
{
    setScene(scene);
}

GraphicsView::~GraphicsView()
{
    delete scene;
}

QRect GraphicsView::getAbsoluteGeometry()
{
    return QRect(mapToGlobal(QPoint(0,0)), geometry().size());
}

QRect GraphicsView::getScreenGeometry()
{
    QDesktopWidget *desktop = QApplication::desktop ();
    QRect screen_geom = desktop->screenGeometry(this);
    QRect ret = getAbsoluteGeometry();

    ret.translate(-screen_geom.topLeft());

    return ret;
}

void GraphicsView::moveEvent()
{
    setupView();
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    setupView();
}

void GraphicsView::moveEvent(QMoveEvent *event)
{
    setupView();
}

void GraphicsView::setupView()
{
    if (fitToScreen) {
        QDesktopWidget *desktop = QApplication::desktop ();
        viewRect = desktop->screenGeometry(this);
        setSceneRect(getScreenGeometry());
    } else {
        viewRect = rect();
        setSceneRect(viewRect);
    }
}
