/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the common GUI parts for multitouch devices
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

#ifndef QDEVICE_H
#define QDEVICE_H

#include <QObject>
#include <QGraphicsScene>
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QSocketNotifier>
#include "udev.h"
#include "touch.h"
#include "kernelDevice.h"
#include "hidmt.h"
#include "graphicsview.h"

namespace Ui {
class Form_Input;
}

class QDevice : public QWidget
{
    Q_OBJECT
public:
    explicit QDevice(KernelDevice *kernelDevice,
                     Udev *udevMgr,
                     UdevDevice *device,
                     GraphicsView *view,
                     QWidget *parent = 0);
    ~QDevice ();

    void createBrushes ();
    float getCoord (float value, float min, float max);
    Touch *getCurrentTouch ();
    QString getName ();
    QString getNode ();

    void processEvent (struct input_event *ev);

private:
    Ui::Form_Input *form;
    KernelDevice *kernelDevice;
    Udev *udevMgr;
    UdevDevice *device;
    UdevDevice *hid;
    HidMT *hid_multitouch;
    GraphicsView *view;
    QHash <int, Touch *> touches;
    QList <QBrush *> brushes;
    QColor refColor;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QBrush *currentBrush;
    QSocketNotifier *sn;

    QBrush *nextBrush();

signals:

public slots:
    void fdReady (int fd);
    void colorClicked ();
    void expertMode (bool value);
};

#endif // QDEVICE_H
