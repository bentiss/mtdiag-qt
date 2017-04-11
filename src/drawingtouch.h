#ifndef DRAWINGTOUCH_H
#define DRAWINGTOUCH_H

#include "touch.h"
#include "deviceview.h"

class DrawingTouch : public Touch
{
public:
    explicit DrawingTouch(DeviceView *view, int radius = 20, QObject *parent = 0);
    virtual ~DrawingTouch();
    virtual bool update(QBrush *color);

private:
    DeviceView *view;
    QGraphicsEllipseItem *ellipse;
    int radius;

signals:

};

#endif // DRAWINGTOUCH_H
