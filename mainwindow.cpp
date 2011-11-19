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

    QList<UdevDevice *> inputDevices = udev->getInputDevices();

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

void MainWindow::addDevice (UdevDevice *device)
{
    KernelDevice *kernelDevice;
    UdevDevice *hid;
    QDevice *qDev;


    kernelDevice = new KernelDevice (device->getDevnode());

    if (!kernelDevice->init())
        return;

    if (!kernelDevice->hasAbs(ABS_MT_SLOT)) {
        delete kernelDevice;
        return;
    }

    hid = udev->getHid(device);

    qDev = new QDevice (kernelDevice, hid, scene, &viewRect, this);
    ui->tabWidgetInputDevices->addTab(qDev, kernelDevice->getName());
    qDevices.append(qDev);
}

void MainWindow::removeDevice (UdevDevice *device)
{
    QString node = device->getDevnode();
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
    UdevDevice *device;
    QString action;
    if (fd != udev->getFd())
        return;

    device = udev->event();

    if (!QString(device->getSysname()).startsWith("event"))
        return;

    action = device->getAction();

    if (action == "add")
        addDevice(device);
    else if (action == "remove")
        removeDevice(device);
}

/*
def __init__(self, qApp):
        QtGui.QMainWindow.__init__(self)
        self.qApp = qApp
        if qApp:
                qApp.installEventFilter (self)
        Ui_MainWindow = uic.loadUiType("mainwindow.ui")[0]
        self.ui = Ui_MainWindow ()
        self.ui.setupUi(self)
        self.scene = QtGui.QGraphicsScene()
        self.ui.graphicsView.setScene(self.scene)
        self.udev = udev.Udev()
        self.view_width = self.view_height = None
        devices = self.udev.get_all_input_devices()
        self.qdevices = []
        self.actions = []
        self.notifiers = {}
        self.xinput_mappings = {}
        self.painter = QtGui.QPainter(self)
        for device in devices:
                self.addDevice(device)
        floating_devices = self.udev.get_unattached_hid_usb_devices()
        if floating_devices.has_key ('046d:c52b'):
                del(floating_devices['046d:c52b'])
        if len(floating_devices) == 0:
                self.ui.menuOrphans_Usb_Hid.setEnabled(False)
        for vidpid, devices in floating_devices.items():
                self.actions.append(VidPidAction (vidpid, devices, self))
        self.addNotifier(self.udev.fileno, self.udev_event)

def eventFilter(self, watched, event):
        if self.qApp != watched or (
                event.type() != QtCore.QEvent.ApplicationActivated and
                event.type() != QtCore.QEvent.ApplicationDeactivated):
                return QtGui.QMainWindow.eventFilter (self, watched, event)

        if event.type() == QtCore.QEvent.ApplicationActivated:
                self.deactivate_xinput()
        elif event.type() == QtCore.QEvent.ApplicationDeactivated:
                self.activate_xinput()
        return True


def disableX11(self, value):
        if value:
                self.deactivate_xinput ()
        else:
                self.activate_xinput ()

def deactivate_xinput (self):
        self.xinput_mappings = {}
        if not self.ui.commandLinkButton_disableX11.isChecked ():
                return
        for qdev in self.qdevices:
                f = os.popen ("xinput list --short '{}'".format(qdev.dev.name), 'r')
                regexp = r'^{}.*id=(\d*).*slave  pointer  \((\d*)\)'.format(qdev.dev.name)
                r = re.compile(regexp)
                for line in f.readlines ():
                        m = r.match(line)
                        if not m:
                                continue
                        id, parent = m.groups()
                        id = int(id)
                        parent = int(parent)
                        self.xinput_mappings[id] = parent
                f.close()
        for id in self.xinput_mappings.keys():
                command = "xinput float {}".format(id)
                print command
                os.system (command)

def activate_xinput (self):
        for id, parent in self.xinput_mappings.items():
                command = "xinput reattach {} {}".format(id, parent)
                print command
                os.system (command)

*/
