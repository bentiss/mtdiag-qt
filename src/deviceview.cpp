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
    devicePrint(new QGraphicsRectItem(0,0,1,1, group)),
    kdev(dev),
    indirect(dev->getIndirect()),
    fitToScreen(indirect),
    aspectRatio(1)
{
    scene->addItem(group);
    devicePrint->setVisible(false);

    bool okX, okY;
    const struct input_absinfo *xAbsInfo = kdev->getAbsinfo(&okX, ABS_X);
    const struct input_absinfo *yAbsInfo = kdev->getAbsinfo(&okY, ABS_Y);

    if (okX && okY) {
        int xRes = xAbsInfo->resolution;
        int yRes = xAbsInfo->resolution;

        if (xRes == 0)
             xRes = 50;
        if (yRes == 0)
             yRes = 50;

        double physicalWidth  = (xAbsInfo->maximum - xAbsInfo->minimum) / xRes;
        double physicalHeight = (yAbsInfo->maximum - yAbsInfo->minimum) / yRes;

        aspectRatio = physicalHeight / physicalWidth;
    }

}

DeviceView::~DeviceView()
{
    delete devicePrint;
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
    this->fitToScreen = fitToScreen;

    /* Direct screen mapping is rather simple, keep it short */
    if (!indirect && fitToScreen) {
        this->viewRect = screenRect;
        group->resetTransform();
        return;
    }

    double width = sceneRect.width();
    double height = sceneRect.height();
    double screenAspectRatio = height/width;

    if (aspectRatio / screenAspectRatio > 1.0) {
        height *= 0.80;
        width = height / aspectRatio;
    } else {
        width *= 0.80;
        height = width * aspectRatio;
    }

    int topX = (int)((sceneRect.width() - width) / 2.0) + sceneRect.x();
    int topY = (int)((sceneRect.height() - height) / 2.0) + sceneRect.y();

    devicePrint->setRect(0, 0, width, height);

    this->viewRect = QRect(0, 0, width, height);
    group->setMatrix(QMatrix(1, 0, 0, 1, topX, topY));
}

void DeviceView::setDevicePrintVisible(bool visible)
{
    if (indirect || !fitToScreen)
        devicePrint->setVisible(visible);
    else
        devicePrint->setVisible(false);
}

