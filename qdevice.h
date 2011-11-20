#ifndef QDEVICE_H
#define QDEVICE_H

#include <QObject>
#include <QGraphicsScene>
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include "udev.h"
#include "touch.h"
#include "kernelDevice.h"

namespace Ui {
class Form_Input;
class Form_Quirks;
}

class QDevice : public QWidget
{
    Q_OBJECT
public:
    explicit QDevice(KernelDevice *kernelDevice,
                     UdevDevice *hid,
                     QGraphicsScene *scene,
                     QRect *sizeWindow,
                     QWidget *parent = 0);
    ~QDevice ();

    void createBrushes ();
    float getCoord (float value, float min, float max);
    Touch *getCurrentTouch ();
    //void close ();
    QString getName ();
    QString getNode ();

    void processEvent (struct input_event *ev);

private:
    Ui::Form_Input *form;
    Ui::Form_Quirks *formQuirks;
    KernelDevice *kernelDevice;
    UdevDevice *hid;
    QGraphicsScene *scene;
    QRect *sizeWindow;
    QHash <int, Touch *> touches;
    QList <QBrush *> brushes;
    QColor refColor;
    QGridLayout *gridLayout;
    QSplitter *splitter;

signals:

public slots:
    void fdReady (int fd);
    void colorClicked ();
    void updateQuirks ();
    void expertMode (bool value);
};

#endif // QDEVICE_H
