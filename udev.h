#ifndef UDEV_H
#define UDEV_H

#include <QObject>
#include <QHash>
#include <libudev.h>
#include "udevdevice.h"

class Udev : public QObject
{
    Q_OBJECT
public:
    explicit Udev (QObject *parent = 0);
    ~Udev();
    int getFd ();
    UdevDevice *event ();
    QHash<QString, QString> getFloatingHidUsbDevices();
    QList<UdevDevice *> getInputDevices();
    UdevDevice *getHid(UdevDevice *device);

private:
    struct udev *udev;
    struct udev_monitor *mon;

signals:

public slots:

};

#endif // UDEV_H
