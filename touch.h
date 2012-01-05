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

#ifndef TOUCH_H
#define TOUCH_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QBrush>

class Touch : public QObject
{
    Q_OBJECT
public:
    explicit Touch(QGraphicsScene *scene, QObject *parent = 0);
    ~Touch();
    bool update(QBrush *color);
    void setCx(int newCx) {cx = newCx;}
    void setCy(int newCy) {cy = newCy;}
    void setTrackingId(int newTrackingId) {trackingId = newTrackingId;}

private:
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    int radius;
    int trackingId;
    int cx;
    int cy;

signals:

public slots:

};

#endif // TOUCH_H
