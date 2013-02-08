/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains a subclass of Touch
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

#include "drawingtouch.h"

static QPen pressedPen = QPen(Qt::black, 5);
static QPen releasedPen = QPen(Qt::black, 1);

DrawingTouch::DrawingTouch(QGraphicsScene *scene, int radius, QObject *parent) :
    Touch(parent),
    scene(scene),
    ellipse(0),
    radius(radius)
{
}

bool DrawingTouch::update(QBrush *color)
{
    bool wasVisible = false;
    if (ellipse) {
        wasVisible = ellipse->isVisible();
        ellipse->setVisible(TrackingId() >= 0);
    }
    else {
        ellipse = scene->addEllipse(QRectF(Cx() - radius, Cy() - radius,
                                           2 * radius, 2 * radius));
    }
    if (TrackingId() >= 0) {
        QRectF rect = ellipse->rect();
        rect.moveCenter(QPointF(Cx(), Cy()));
        ellipse->setRect(rect);
        ellipse->setPen(Pressed() ? pressedPen : releasedPen);
        if (!wasVisible) {
            ellipse->setBrush(*color);
            ellipse->show();
            return true;
        }
   }

    return false;
}

DrawingTouch::~DrawingTouch()
{
    scene->removeItem(ellipse);
    delete ellipse;
}
