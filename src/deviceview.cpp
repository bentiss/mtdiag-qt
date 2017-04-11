/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Red Hat, Inc (2017)
 **
 **	this files contains the kernel access to a multitouch device
 **
 **	Contributors:
 **		Benjamin Tissoires <benjamin.tissoires@gmail.com>
 **
 **
 **	This program is provided to you as free software;
 **	you can redistribute it	and/or modify it under the terms of the
 **	GNU General Public License as published by the Free Software
 **	Foundation; either version 2 of the License, or (at your option)
 **	any later version.
 **/

#include "deviceview.h"

DeviceView::DeviceView(QGraphicsScene *scene, KernelDevice *dev, QObject *parent) :
    QObject(parent),
    scene(scene),
    group(new QGraphicsItemGroup()),
    indirect(dev->getIndirect())
{
    scene->addItem(group);
}

DeviceView::~DeviceView()
{

}

void DeviceView::removeItem(QGraphicsItem *item)
{
    scene->removeItem(item);
}

QGraphicsEllipseItem *DeviceView::addEllipse(const QRectF & rect)
{
    return new QGraphicsEllipseItem(rect, group);
}

void DeviceView::setupView(QRect screenRect, QRect sceneRect, bool fitToScreen)
{
    if (!indirect && fitToScreen) {
        this->viewRect = screenRect;
        group->resetTransform();
    } else {
        this->viewRect = QRect(0, 0, sceneRect.width(), sceneRect.height());
        group->setMatrix(QMatrix(1, 0, 0, 1, sceneRect.x(), sceneRect.y()));
    }
}

