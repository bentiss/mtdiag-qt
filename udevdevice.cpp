#include "udevdevice.h"
#include <QDebug>


QHash<struct udev_device *, UdevDevice *> UdevDevice::devices =
        QHash<struct udev_device *, UdevDevice *>();

UdevDevice::UdevDevice(struct udev_device *device):
    device(device)
{
}

UdevDevice *UdevDevice::getDevice (struct udev_device *device)
{
    if (!device)
        return 0;

    if (!UdevDevice::devices.contains(device))
        UdevDevice::devices[device] = new UdevDevice(device);
    return UdevDevice::devices[device];
}

void UdevDevice::cleanDevicesList (struct udev *udev)
{
    QHashIterator<struct udev_device *, UdevDevice *> i(devices);
    QList<struct udev_device *> toRemove = QList<struct udev_device *>();
    while (i.hasNext()) {
        i.next();
        if (i.value()->udev() == udev) {
            toRemove.append(i.key());
        }
    }
    for (int i = 0; i < toRemove.count(); ++i) {
        struct udev_device *udev_device = toRemove[i];
        UdevDevice *dev = devices[udev_device];
        UdevDevice *inputParent = dev->getParentWithSubsystemDevtype("input", 0);
        devices.remove(udev_device);
        if (inputParent) {
            if (!toRemove.contains(inputParent->device)) {
                udev_device_unref(udev_device);
            }
        } else {
            udev_device_unref(udev_device);
        }
        delete dev;
    }

}
