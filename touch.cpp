#include "touch.h"

Touch::Touch(QGraphicsScene *scene, QBrush *brush, QObject *parent) :
    QObject(parent),
    scene(scene),
    brush(brush),
    ellipse(0),
    radius(20),
    trackingId(-1),
    cx(0),
    cy(0)
{
}

bool Touch::update()
{
    if (trackingId >= 0) {
        if (ellipse) {
            QRectF rect = ellipse->rect();
            rect.moveCenter(QPointF(cx, cy));
            ellipse->setRect(rect);
        } else {
            ellipse = scene->addEllipse(QRectF(cx - radius, cy - radius,
                                               2 * radius, 2 * radius),
                                        QPen(),
                                        *brush);
        }
    } else {
        if (ellipse) {
            scene->removeItem(ellipse);
            delete ellipse;
            ellipse = 0;
        }
        return false;
    }
    return true;
}
