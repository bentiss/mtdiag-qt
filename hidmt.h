/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the GUI for hid-multitouch devices
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

#ifndef HIDMT_H
#define HIDMT_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QSocketNotifier>
#include "udev.h"
#include <linux/hiddev.h>

namespace Ui {
class Form_Quirks;
class Form_Protocol;
}

namespace MT {
struct touch {
    signed int *inRange;
    signed int *tipSwitch;
    signed int *confidence;
    signed int *contactID;
    signed int *pressure;
    signed int *x;
    signed int *y;
    signed int *width;
    signed int *height;
};
}

class HidMT : public QWidget
{
    Q_OBJECT
public:
    explicit HidMT(Udev *udevMgr, UdevDevice *hiddev, QWidget *parent = 0);
    ~HidMT();

    void expertMode (bool value);

private:
    Ui::Form_Quirks *formQuirks;
    Ui::Form_Protocol *formProtocol;
    Udev *udevMgr;
    UdevDevice *hid;
    UdevDevice *hiddev;
    QString quirksPath;
    QSocketNotifier *sn;
    struct hiddev_event *usages;
    struct hiddev_event *prev_usages;
    int usagesCount;
    QList<struct MT::touch> mt_values;
    QList<struct MT::touch> prev_mt_values;
    int contactCount;
    int fdHiddev;
    int eventIndex;

    uint readQuirks();
    void initDebugfsEvents ();
    bool openDebugfsEvents ();
    bool wasManuallyAdded ();
    void processOneEvent(struct hiddev_event *ev);
    void processOneFrame();

signals:

public slots:
    void updateQuirks ();
    void autoDetect (bool state);
    void events(int fd);
};

#endif // HIDMT_H
