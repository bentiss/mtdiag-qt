#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QEvent>
#include <QGraphicsScene>
#include <QSocketNotifier>
#include <QDebug>
#include "udev.h"
#include "qdevice.h"
#include "xi2manager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QApplication *rootApp, QWidget *parent = 0);
    ~MainWindow();

    bool isFitToScreen();
    bool isFullScreen ();
    bool isMaximized ();

private:
    Ui::MainWindow *ui;
    QApplication *rootApp;
    Udev *udev;
    QList<QDevice *> qDevices;
    QSocketNotifier *sn;
    XI2Manager *xi2manager;

    void addDevice (UdevDevice *device);
    void removeDevice (UdevDevice *device);

protected:
    void moveEvent(QMoveEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

public slots:
    void fullscreen (bool value);
    void disableX11 (bool value);
    void udevEvent(int fd);

};

#endif // MAINWINDOW_H
