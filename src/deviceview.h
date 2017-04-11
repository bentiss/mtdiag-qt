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

#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include "kernelDevice.h"

class DeviceView : public QObject
{
    Q_OBJECT
public:
    explicit DeviceView(QGraphicsScene *scene, KernelDevice *dev, QObject *parent = 0);
    ~DeviceView();

    int viewWidth() { return viewRect.width(); }
    int viewHeight() { return viewRect.height(); }
    void removeItem(QGraphicsItem *item);
    QGraphicsEllipseItem *addEllipse(const QRectF & rect);

    void setupView(QRect screenRect, QRect sceneRect, bool fitToScreen);
private:
    QGraphicsScene *scene;
    QGraphicsItemGroup *group;
    bool indirect;
    QRect viewRect;

signals:

public slots:

};

#endif // DEVICEVIEW_H
