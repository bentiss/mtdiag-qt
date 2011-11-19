#include "udevdevice.h"
#include <QDebug>
#include <libudev.h>


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

const char *UdevDevice::getDevpath() {return udev_device_get_devpath(device);}
const char *UdevDevice::getSubsystem() {return udev_device_get_subsystem(device);}
const char *UdevDevice::getDevtype() {return udev_device_get_devtype(device);}
const char *UdevDevice::getSyspath() {return udev_device_get_syspath(device);}
const char *UdevDevice::getSysname() {return udev_device_get_sysname(device);}
const char *UdevDevice::getSysnum() {return udev_device_get_sysnum(device);}
const char *UdevDevice::getDevnode() {return udev_device_get_devnode(device);}

struct udev_list_entry *UdevDevice::getDevlinksListEntry() {
    return udev_device_get_devlinks_list_entry(device);}
struct udev_list_entry *UdevDevice::getPropertiesListEntry() {
    return udev_device_get_properties_list_entry(device);}
struct udev_list_entry *UdevDevice::getTagsListEntry() {
    return udev_device_get_tags_list_entry(device);}
const char *UdevDevice::getPropertyValue(const char *key) {
    return udev_device_get_property_value(device, key);}
const char *UdevDevice::getDriver()
{
    return udev_device_get_driver(device);}
/*dev_t UdevDevice::getDevnum()
{
    return udev_device_get_devnum(device);
}*/
const char *UdevDevice::getAction() {
    return udev_device_get_action(device);}
unsigned long long int UdevDevice::getSeqnum() {
    return udev_device_get_seqnum(device);}
const char *UdevDevice::getSysattrValue(const char *sysattr) {
    return udev_device_get_sysattr_value(device, sysattr);}

UdevDevice *UdevDevice::getParent() {
    return getDevice(udev_device_get_parent(device));}
UdevDevice *UdevDevice::getParentWithSubsystemDevtype(const char *subsystem,
                                          const char *devtype) {
    return getDevice(
                udev_device_get_parent_with_subsystem_devtype(device,
                                                              subsystem,
                                                              devtype));}

UdevDevice *UdevDevice::getDevice (struct udev *udev, const char *path)
{
    return UdevDevice::getDevice(udev_device_new_from_syspath(udev, path));
}

struct udev *UdevDevice::udev()
{
    return udev_device_get_udev(device);
}
