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
