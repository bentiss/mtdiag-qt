#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSocketNotifier>
#include "udev.h"
#include "qdevice.h"

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

    const QRect getViewRect () { return viewRect ; }

private:
    Ui::MainWindow *ui;
    QApplication *rootApp;
    Udev *udev;
    QList<QDevice *> qDevices;
    QGraphicsScene *scene;
    QRect viewRect;
    QSocketNotifier *sn;

    void addDevice (struct udev_device *device);
    void removeDevice (struct udev_device *device);

protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);

public slots:
    void fullscreen (bool value);
    void disableX11 (bool value);
    void setupDisplayView ();
    void udevEvent(int fd);

};

#endif // MAINWINDOW_H
