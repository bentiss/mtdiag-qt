/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the common GUI parts for multitouch devices
 **
 **	Contributors:
 **		Benjamin Tissoires <benjamin.tissoires@gmail.com>
 **
 **
 **	This program is provided to you as free software;
 **	you can redistribute it	and/or modify it under the terms of the
 **	GNU General Public License as published by the Free Software
 **	Foundation; either version 2 of the License, or (at your option)
 **	any later version.
 **/

#include "qdevice.h"
#include "ui_input.h"
#include "mainwindow.h"
#include "drawingtouch.h"
#include <QDebug>
#include <QColorDialog>
#include <stdlib.h>

extern "C" {
#include <linux/input.h>
}

static QList<QDevice *> devices = QList<QDevice *> ();

static void staticProcessEvent (struct input_event *ev, void *args)
{
    QDevice *obj = (QDevice *)args;
    obj->processEvent(ev);
}

QDevice::QDevice(KernelDevice *kernelDevice,
                 Udev *udevMgr,
                 UdevDevice *device,
                 GraphicsView* view,
                 QWidget *parent) :
    QWidget(parent),
    form(new Ui::Form_Input),
    kernelDevice(kernelDevice),
    udevMgr(udevMgr),
    device(device),
    hid(udevMgr->getHid(device)),
    hid_multitouch(0),
    view(view->newGroup(kernelDevice))
{
    int fd = kernelDevice->getFileDescriptor();
    int hue;

    touches = QHash <int, Touch *>();
    brushes = QList <QBrush *>();
    hue = (rand() % 10) * 36;
    refColor = QColor::fromHsv (hue, 128, 255);
    pointerBrush = new QBrush(QColor::fromRgbF(0, 0, 0, 0));
    pointer = new DrawingTouch (this->view, 30);

    gridLayout = new QGridLayout(this);
    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    gridLayout->addWidget(splitter, 0, 0, 1, 1);

    form->setupUi(splitter);
    createBrushes ();
    if (hid)
        form->lineEdit_driver->setText(hid->getDriver());
    else
        form->lineEdit_driver->setEnabled(false);
    form->lineEdit_node->setText(kernelDevice->getPath());

    if (hid && QString("hid-multitouch") == hid->getDriver() && kernelDevice->hasAbs(ABS_MT_SLOT)) {
        hid_multitouch = new HidMT(udevMgr, hid, this->splitter);
    }

    kernelDevice->setProcessEventArgs(&staticProcessEvent, this);
    sn = new QSocketNotifier(fd, QSocketNotifier::Read, this);

    QObject::connect(form->colorWidget, SIGNAL(doubleClicked()),
                     this, SLOT(colorClicked()));
    QObject::connect(sn, SIGNAL(activated(int)), this, SLOT(fdReady(int)));
    devices.append(this);
}

QDevice::~QDevice ()
{
    devices.removeOne(this);
    delete sn;
    delete hid;
    delete kernelDevice;
    delete form;
    if (hid_multitouch)
        delete hid_multitouch;
    foreach (Touch *touch, touches)
        delete touch;
    delete pointer;
    foreach (QBrush *brush, brushes)
        delete brush;
    delete pointerBrush;
}

void QDevice::processEvent (struct input_event *ev)
{
    switch (ev->type) {
    case EV_ABS:
    {
        Touch *touch;
        bool ok;
        const struct input_absinfo *absinfo = kernelDevice->getAbsinfo(&ok, ev->code);
        switch (ev->code) {
        case ABS_X:
            pointer->setCx(getCoord (ev->value,
                                     absinfo->minimum,
                                     absinfo->maximum) * view->viewWidth());
            break;
        case ABS_Y:
            pointer->setCy(getCoord (ev->value,
                                     absinfo->minimum,
                                     absinfo->maximum) * view->viewHeight());
            break;
        case ABS_PRESSURE:
            pointer->setPressed(ev->value > 0);
            break;
        case ABS_MT_POSITION_X:
            touch = getCurrentTouch();
            touch->setCx(getCoord (ev->value,
                                   absinfo->minimum,
                                   absinfo->maximum) * view->viewWidth());
            break;
        case ABS_MT_POSITION_Y:
            touch = getCurrentTouch();
            touch->setCy(getCoord (ev->value,
                                   absinfo->minimum,
                                   absinfo->maximum) * view->viewHeight());
            break;
        case ABS_MT_TRACKING_ID:
            touch = getCurrentTouch();
            touch->setTrackingId(ev->value);
            break;
        case ABS_MT_DISTANCE:
            touch = getCurrentTouch();
            touch->setPressed(ev->value == 0);
            break;
        }
    }
        break;
    case EV_KEY:
    {
        switch (ev->code) {
        case BTN_TOUCH:
            pointer->setPressed(ev->value > 0);
            if (!kernelDevice->hasKey(BTN_TOOL_PEN))
                pointer->setTrackingId(ev->value ? 0 : -1);
            break;
        case BTN_TOOL_PEN:
            pointer->setTrackingId(ev->value ? 0 : -1);
            break;
        case BTN_LEFT:
            pointer->setTrackingId(ev->value ? 0 : -1);
            break;
        }
    }
        break;
    case EV_SYN:
        if (ev->code == SYN_REPORT) {
            unsigned int count = 0;
            pointer->update(pointerBrush);
            foreach (Touch *touch, touches) {
                if (touch->active())
                    ++count;
                if (touch->update (currentBrush))
                    currentBrush = nextBrush();
            }
            foreach (QDevice *d, devices) {
                d->view->setDevicePrintVisible(d == this);
            }
            break;
        }
    }

}

void QDevice::fdReady(int fd)
{
    if (fd == kernelDevice->getFileDescriptor())
        kernelDevice->event();
}

QString QDevice::getName ()
{
    return QString(kernelDevice->getName());
}

QString QDevice::getNode ()
{
    return QString(kernelDevice->getPath());
}

float QDevice::getCoord (float value, float min, float max)
{
    return (value - min) / (max - min);
}

QBrush *QDevice::nextBrush()
{
    QBrush *brush = brushes[0];
    brushes.pop_front();
    brushes.append(brush);
    return brush;
}

Touch *QDevice::getCurrentTouch ()
{
    bool ok;
    int slot = kernelDevice->getAbsinfo(&ok, ABS_MT_SLOT)->value;
    /* no need to test ok */

    if (!touches.contains(slot)) {
        touches[slot] = new DrawingTouch (view);
        touches[slot]->setPressed(kernelDevice->getInitialInRangeState(slot) == 0);
    }
    return touches[slot];
}

void QDevice::createBrushes ()
{
    form->colorWidget->setColor (refColor);
    for (int i = 0; i < brushes.count(); ++i) {
        delete brushes[i];
    }
    brushes.clear();
    for (int i = 80 ; i <= 120; i += 10) {
        QColor color = refColor.lighter (i);
        brushes.append(new QBrush(color));
    }
    currentBrush = nextBrush();
}

void QDevice::colorClicked ()
{
    QColor color = QColorDialog::getColor (refColor, this,
                                           QString("Choose a color"),
                                           QColorDialog::ShowAlphaChannel);
    if (!color.isValid())
        return;
    refColor = color;
    createBrushes();
}

void QDevice::expertMode (bool value)
{
    if (!hid_multitouch)
        return;

    hid_multitouch->expertMode(value);
}
