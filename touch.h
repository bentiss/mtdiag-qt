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
    explicit Touch(QGraphicsScene *scene, QBrush *brush,QObject *parent = 0);
    ~Touch();
    void update();
    void setCx(int newCx) {cx = newCx;}
    void setCy(int newCy) {cy = newCy;}
    void setTrackingId(int newTrackingId) {trackingId = newTrackingId;}

private:
    QGraphicsScene *scene;
    QBrush *brush;
    QGraphicsEllipseItem *ellipse;
    int radius;
    int trackingId;
    int cx;
    int cy;

signals:

public slots:

};

#endif // TOUCH_H
