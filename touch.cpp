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
