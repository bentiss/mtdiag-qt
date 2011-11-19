#include "udev.h"
#include <stdlib.h>
#include <QDebug>
#include <QStringList>

static void cleanUdevDevices (struct udev *udev, const char *subsystem);

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
    cleanUdevDevices (udev, "input");
    cleanUdevDevices (udev, "hid");
    udev_unref(udev);
    udev_monitor_unref(mon);
}

static void cleanUdevDevices (struct udev *udev, const char *subsystem)
{
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, subsystem);
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);
    /* For each item enumerated, print out its information.
       udev_list_entry_foreach is a macro which expands to
       a loop. The loop will be executed for each member in
       devices, setting dev_list_entry to a list entry
       which contains the device's path in /sys. */
    udev_list_entry_foreach(dev_list_entry, devices) {
        const char *path;
        /* Get the filename of the /sys entry for the device
           and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        udev_device_unref(dev);

    }
    udev_enumerate_unref(enumerate);
}

int Udev::getFd ()
{
    return udev_monitor_get_fd(mon);
}

struct udev_device *Udev::event ()
{
    return udev_monitor_receive_device(mon);
}

QHash<QString, QString> Udev::getFloatingHidUsbDevices()
{
    QHash<QString, QString> floatingDevices;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev, *parent;

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
        dev = udev_device_new_from_syspath(udev, path);

        /* usb_device_get_devnode() returns the path to the device node
           itself in /dev. */
        if (udev_device_get_driver(dev)) {
            //udev_device_unref(dev);
            continue;
        }

        /* The device pointed to by dev contains information about
           the hidraw device. In order to get information about the
           USB device, get the parent device with the
           subsystem/devtype pair of "usb"/"usb_device". This will
           be several levels up the tree, but the function will find
           it.*/
        parent = udev_device_get_parent_with_subsystem_devtype(
                               dev,
                               "usb",
                               "usb_device");
        if (!parent) {
                qDebug() << "Unable to find parent usb device.\n";
                continue;
        }

        vidpid = udev_device_get_sysattr_value(parent,"idVendor");
        vidpid.append(":");
        vidpid.append(udev_device_get_sysattr_value(parent, "idProduct"));

        floatingDevices[vidpid] = udev_device_get_sysattr_value(parent,
                                        "manufacturer");
        floatingDevices[vidpid].append(" ");
        floatingDevices[vidpid].append(udev_device_get_sysattr_value(parent,
                                        "product"));

        udev_device_unref(parent);

    }
    udev_enumerate_unref(enumerate);
    return floatingDevices;
}

QList<struct udev_device *> Udev::getInputDevices()
{
    QList<struct udev_device *> inputDevices;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

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
        dev = udev_device_new_from_syspath(udev, path);

        if (!udev_device_get_devnode(dev)) {
            udev_device_unref(dev);
            continue;
        }

        sysName = udev_device_get_sysname(dev);
        if (sysName.startsWith("event"))
            inputDevices.append(dev);
    }
    udev_enumerate_unref(enumerate);
    return inputDevices;
}

struct udev_device *Udev::getHid(struct udev_device *device)
{
    struct udev_device *parent = udev_device_get_parent(device);
    QString product = udev_device_get_property_value(parent, "PRODUCT");
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *retValue = 0;
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
        struct udev_device *dev;
        const char *path;
        QString sysName;
        /* Get the filename of the /sys entry for the device
           and create a udev_device object (dev) representing it */
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        sysName = udev_device_get_sysname(dev);
        if (sysName.contains(magic)) {
            retValue = dev;
            break;
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    return retValue;
}
