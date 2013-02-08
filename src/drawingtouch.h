#ifndef DRAWINGTOUCH_H
#define DRAWINGTOUCH_H

#include "touch.h"

class DrawingTouch : public Touch
{
public:
    explicit DrawingTouch(QGraphicsScene *scene, int radius = 20, QObject *parent = 0);
    virtual ~DrawingTouch();
    virtual bool update(QBrush *color);

private:
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    int radius;

signals:

};

#endif // DRAWINGTOUCH_H
