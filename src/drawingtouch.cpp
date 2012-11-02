#include "drawingtouch.h"

DrawingTouch::DrawingTouch(QGraphicsScene *scene, QObject *parent) :
    Touch(parent),
    scene(scene),
    ellipse(0),
    radius(20)
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
