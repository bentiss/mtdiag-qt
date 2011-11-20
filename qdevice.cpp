#include "qdevice.h"
#include "ui_input.h"
#include "ui_quirks.h"
#include "mainwindow.h"
#include <QDebug>
#include <QSocketNotifier>
#include <QColorDialog>
#include <stdlib.h>

extern "C" {
#include <linux/input.h>
}

static void staticProcessEvent (struct input_event *ev, void *args)
{
    QDevice *obj = (QDevice *)args;
    obj->processEvent(ev);
}

QDevice::QDevice(KernelDevice *kernelDevice,
                 UdevDevice *hid,
                 QGraphicsScene *scene,
                 QRect *sizeWindow,
                 QWidget *parent) :
    QWidget(parent),
    form(new Ui::Form_Input),
    formQuirks(new Ui::Form_Quirks),
    kernelDevice(kernelDevice),
    hid(hid),
    scene(scene),
    sizeWindow(sizeWindow)
{
    int fd = kernelDevice->getFileDescriptor();
    int hue;
    QSocketNotifier *sn;
    touches = QHash <int, Touch *>();
    brushes = QList <QBrush *>();
    hue = (rand() % 10) * 36;
    refColor = QColor::fromHsv (hue, 128, 255);

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

    //if (QString("hid-multitouch") == udev_device_get_driver(hid)) {
        formQuirks->setupUi(this);
        splitter->addWidget(formQuirks->dockWidget_quirks);
    //}

    kernelDevice->setProcessEventArgs(&staticProcessEvent, this);
    sn = new QSocketNotifier(fd, QSocketNotifier::Read, this);

    QObject::connect(form->colorWidget, SIGNAL(doubleClicked()),
                     this, SLOT(colorClicked()));
    QObject::connect(sn, SIGNAL(activated(int)), this, SLOT(fdReady(int)));
}

QDevice::~QDevice ()
{
    delete hid;
    delete kernelDevice;
    delete form;
    delete formQuirks;
    for (int i = 0; i < brushes.count(); ++i) {
        delete brushes[i];
    }
}

void QDevice::processEvent (struct input_event *ev)
{
    if (ev->type == EV_ABS) {
        Touch *touch;
        bool ok;
        struct input_absinfo *absinfo = kernelDevice->getAbsinfo(&ok, ev->code);
        switch (ev->code) {
        case ABS_MT_POSITION_X:
            touch = getCurrentTouch();
            touch->setCx(getCoord (ev->value,
                                   absinfo->minimum,
                                   absinfo->maximum) * sizeWindow->width());
            break;
        case ABS_MT_POSITION_Y:
            touch = getCurrentTouch();
            touch->setCy(getCoord (ev->value,
                                   absinfo->minimum,
                                   absinfo->maximum) * sizeWindow->height());
            break;
        case ABS_MT_TRACKING_ID:
            touch = getCurrentTouch();
            touch->setTrackingId(ev->value);
            break;
        }
    } else if (ev->type == EV_SYN && ev->code == SYN_REPORT) {
        QList<int> keys = touches.keys();
        for (int i = 0; i < keys.count(); ++i) {
            touches[keys[i]]->update ();
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

Touch *QDevice::getCurrentTouch ()
{
    bool ok;
    int slot = kernelDevice->getAbsinfo(&ok, ABS_MT_SLOT)->value;
    /* no need to test ok */

    if (!touches.contains(slot)) {
        QBrush *brush = brushes[0];
        brushes.pop_front();
        brushes.append(brush);
        touches[slot] = new Touch (scene, brush);
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

void QDevice::updateQuirks ()
{

}
