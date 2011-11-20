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

void Touch::update()
{
    if (ellipse)
        ellipse->setVisible(trackingId >= 0);
    else
        ellipse = scene->addEllipse(QRectF(cx - radius, cy - radius,
                                           2 * radius, 2 * radius),
                                    QPen(),
                                    *brush);
    if (trackingId >= 0) {
        QRectF rect = ellipse->rect();
        rect.moveCenter(QPointF(cx, cy));
        ellipse->setRect(rect);
        ellipse->show();
   }
}

Touch::~Touch()
{
    scene->removeItem(ellipse);
    delete ellipse;
}
