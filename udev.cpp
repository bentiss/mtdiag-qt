#include "udev.h"
#include <stdlib.h>
#include <QDebug>
#include <QStringList>
#include <libudev.h>

Udev::Udev(QObject *parent) :
    QObject(parent)
{
    udev = udev_new();
    if (!udev) {
        // TODO error handling
        qDebug() << "Can't create udev\n";
        exit(1);
    }

    /* Set up a monitor to monitor hidraw devices */
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "input", NULL);
    udev_monitor_enable_receiving(mon);
}

Udev::~Udev()
{
    UdevDevice::cleanDevicesList(udev);
    udev_unref(udev);
    udev_monitor_unref(mon);
}

static void cleanUdevDevices (struct udev *udev, const char *subsystem)
{
}

int Udev::getFd ()
{
    return udev_monitor_get_fd(mon);
}

UdevDevice *Udev::event ()
{
    return UdevDevice::getDevice(udev_monitor_receive_device(mon));
}

QHash<QString, QString> Udev::getFloatingHidUsbDevices()
{
    QHash<QString, QString> floatingDevices;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    UdevDevice *dev, *parent;

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "hid");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
       udev_list_entry_foreach is a macro which expands to
       a loop. The loop will be executed for each member in
       devices, setting dev_list_entry to a list entry
       which contains the device's path in /sys. */
    udev_list_entry_foreach(dev_list_entry, devices) {
        const char *path;
        QString vidpid;
        /* Get the filename of the /sys entry for the device
           and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = UdevDevice::getDevice(udev, path);

        /* usb_device_get_devnode() returns the path to the device node
           itself in /dev. */
        if (dev->getDriver()) {
            //udev_device_unref(dev);
            continue;
        }

        /* The device pointed to by dev contains information about
           the hidraw device. In order to get information about the
           USB device, get the parent device with the
           subsystem/devtype pair of "usb"/"usb_device". This will
           be several levels up the tree, but the function will find
           it.*/
        parent = dev->getParentWithSubsystemDevtype ("usb", "usb_device");
        if (!parent) {
                qDebug() << "Unable to find parent usb device.\n";
                continue;
        }

        vidpid = parent->getSysattrValue ("idVendor");
        vidpid.append(":");
        vidpid.append(parent->getSysattrValue ("idProduct"));

        floatingDevices[vidpid] = parent->getSysattrValue ("manufacturer");
        floatingDevices[vidpid].append(" ");
        floatingDevices[vidpid].append(parent->getSysattrValue ("product"));
    }
    return floatingDevices;
}

QList<UdevDevice *> Udev::getInputDevices()
{
    QList<UdevDevice *> inputDevices;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    UdevDevice *dev;

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "input");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
       udev_list_entry_foreach is a macro which expands to
       a loop. The loop will be executed for each member in
       devices, setting dev_list_entry to a list entry
       which contains the device's path in /sys. */
    udev_list_entry_foreach(dev_list_entry, devices) {
        const char *path;
        QString sysName;
        /* Get the filename of the /sys entry for the device
           and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = UdevDevice::getDevice(udev, path);

        if (!dev->getDevnode())
            continue;

        sysName = dev->getSysname();
        if (sysName.startsWith("event"))
            inputDevices.append(dev);
    }
    return inputDevices;
}

UdevDevice *Udev::getHid(UdevDevice *device)
{
    UdevDevice *parent = device->getParent();
    QString product = parent->getPropertyValue("PRODUCT");
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    UdevDevice *retValue = 0;
    QStringList productList = product.split("/");
    QString bus = productList[0].toUpper().rightJustified(4, '0');
    QString vid = productList[1].toUpper().rightJustified(4, '0');
    QString pid = productList[2].toUpper().rightJustified(4, '0');
    QString magic = QString("%1:%2:%3").arg(bus).arg(vid).arg(pid);

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "hid");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
       udev_list_entry_foreach is a macro which expands to
       a loop. The loop will be executed for each member in
       devices, setting dev_list_entry to a list entry
       which contains the device's path in /sys. */
    udev_list_entry_foreach(dev_list_entry, devices) {
        UdevDevice *dev;
        const char *path;
        QString sysName;
        /* Get the filename of the /sys entry for the device
           and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = UdevDevice::getDevice(udev, path);

        sysName = dev->getSysname();
        if (sysName.contains(magic)) {
            retValue = dev;
            break;
        }
    }
    return retValue;
}
