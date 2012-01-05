/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the drawing section of a touch
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

#include "touch.h"

Touch::Touch(QGraphicsScene *scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    ellipse(0),
    radius(20),
    trackingId(-1),
    cx(0),
    cy(0)
{
}

bool Touch::update(QBrush *color)
{
    bool wasVisible = false;
    if (ellipse) {
        wasVisible = ellipse->isVisible();
        ellipse->setVisible(trackingId >= 0);
    }
    else {
        ellipse = scene->addEllipse(QRectF(cx - radius, cy - radius,
                                           2 * radius, 2 * radius));
    }
    if (trackingId >= 0) {
        QRectF rect = ellipse->rect();
        rect.moveCenter(QPointF(cx, cy));
        ellipse->setRect(rect);
        if (!wasVisible) {
            ellipse->setBrush(*color);
            ellipse->show();
            return true;
        }
   }

    return false;
}

Touch::~Touch()
{
    scene->removeItem(ellipse);
    delete ellipse;
}
