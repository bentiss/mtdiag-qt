/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the udev C++ layer
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

#ifndef UDEV_H
#define UDEV_H

#include <QObject>
#include <QHash>
#include "udevdevice.h"

struct udev;
struct udev_monitor;

class Udev : public QObject
{
    Q_OBJECT
public:
    explicit Udev (QObject *parent = 0);
    ~Udev();
    int getFd ();
    UdevDevice *event ();
    QList<QString> getFloatingHidUsbDevices();
    QList<UdevDevice *> getInputDevices();
    UdevDevice *getHid(UdevDevice *device);
    UdevDevice *getHiddev(UdevDevice *hidDevice);

private:
    struct udev *udev;
    struct udev_monitor *mon;

signals:

public slots:

};

#endif // UDEV_H
