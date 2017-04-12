/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the main window GUI
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
#include "vidpidaction.h"

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
    QList<VidPidAction *> actions;

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
