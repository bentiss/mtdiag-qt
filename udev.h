#ifndef UDEV_H
#define UDEV_H

#include <QObject>
#include <QHash>
#include <libudev.h>

class Udev : public QObject
{
    Q_OBJECT
public:
    explicit Udev (QObject *parent = 0);
    ~Udev();
    int getFd ();
    struct udev_device *event ();
    QHash<QString, QString> getFloatingHidUsbDevices();
    QList<struct udev_device *> getInputDevices();
    struct udev_device *getHid(struct udev_device *device);

private:
    struct udev *udev;
    struct udev_monitor *mon;

signals:

public slots:

};

#endif // UDEV_H
