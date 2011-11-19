#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include "kernelDevice.h"

extern "C" {
#include <linux/input.h>
}

MainWindow::MainWindow(QApplication *rootApp, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rootApp(rootApp),
    udev(new Udev()),
    qDevices(QList<QDevice *>()),
    scene(new QGraphicsScene ())
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    QList<struct udev_device *> inputDevices = udev->getInputDevices();

    for (int i = 0; i < inputDevices.count(); ++i)
        addDevice(inputDevices[i]);

    sn = new QSocketNotifier(udev->getFd(), QSocketNotifier::Read, this);
    QObject::connect(sn, SIGNAL(activated(int)), this, SLOT(udevEvent(int)));
}

MainWindow::~MainWindow()
{
    delete sn;
    delete scene;
    delete udev;
}

bool MainWindow::isFitToScreen()
{
    return ui->commandLinkButton_fit_to_screen->isChecked();
}

bool MainWindow::isFullScreen ()
{
    return ui->commandLinkButton_fullscreen->isChecked();
}

bool MainWindow::isMaximized ()
{
    return windowState() & Qt::WindowMaximized;
}

void MainWindow::addDevice (struct udev_device *device)
{
    KernelDevice *kernelDevice;
    struct udev_device *hid;
    QDevice *qDev;


    kernelDevice = new KernelDevice (udev_device_get_devnode(device));

    if (!kernelDevice->init())
        return;

    if (!kernelDevice->hasAbs(ABS_MT_SLOT)) {
        delete kernelDevice;
        return;
    }

    qDebug() << kernelDevice->getName();

    hid = udev->getHid(device);

    qDev = new QDevice (kernelDevice, hid, scene, &viewRect, this);
    ui->tabWidgetInputDevices->addTab(qDev, kernelDevice->getName());
    qDevices.append(qDev);
}

void MainWindow::removeDevice (struct udev_device *device)
{
    QString node = udev_device_get_devnode(device);
    QDevice *matchedQDev = 0;
    int index = -1;

    for (int i = 0; i < qDevices.count(); ++i) {
        if (node == qDevices[i]->getNode()) {
            index = i;
            matchedQDev = qDevices[i];
        }
    }

    if (index < 0)
        return;

    qDevices.removeAt(index);
    index = ui->tabWidgetInputDevices->indexOf(matchedQDev);
    ui->tabWidgetInputDevices->removeTab(index);
    delete matchedQDev;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    setupDisplayView ();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    setupDisplayView ();
}

void MainWindow::fullscreen (bool value)
{
    if (value)
        setWindowState (windowState() | Qt::WindowFullScreen);
    else
        setWindowState (windowState() & ~Qt::WindowFullScreen);
}

void MainWindow::disableX11 (bool value)
{
    /*if value:
            self.deactivate_xinput ()
    else:
            self.activate_xinput ()*/
}

void MainWindow::setupDisplayView ()
{
    int x = 0;
    int y = 0;

    if (isFitToScreen()) {
        QDesktopWidget *desktop = rootApp->desktop();
        QRect screen_geom = desktop->screenGeometry(ui->graphicsView);
        viewRect = screen_geom;
        x += ui->graphicsView->x() + ui->centralwidget->x();
        y += ui->graphicsView->y() + ui->centralwidget->y();
        if (!isFullScreen() && !isMaximized()) {
            x += geometry().x();
            y += geometry().y();
            x -= screen_geom.x();
            y -= screen_geom.y();
        }
        if (isMaximized()) {
            int dx = (viewRect.width() - width()) / 2;
            int dy = (viewRect.height() - height());
            x += dx;
            y += dy;
        }
    } else {
        viewRect = ui->graphicsView->rect();
    }
    viewRect.moveTo(x, y);
    ui->graphicsView->setSceneRect(viewRect);
}

void MainWindow::udevEvent(int fd)
{
    struct udev_device *device;
    QString action;
    if (fd != udev->getFd())
        return;

    device = udev->event();

    if (!QString(udev_device_get_sysname(device)).startsWith("event"))
        return;

    action = udev_device_get_action(device);

    if (action == "add")
        addDevice(device);
    else if (action == "remove")
        removeDevice(device);
}
