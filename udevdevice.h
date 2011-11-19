#ifndef UDEVDEVICE_H
#define UDEVDEVICE_H
#include <libudev.h>
#include <QHash>

class UdevDevice
{
public:
    const char *getDevpath() {return udev_device_get_devpath(device);}
    const char *getSubsystem() {return udev_device_get_subsystem(device);}
    const char *getDevtype() {return udev_device_get_devtype(device);}
    const char *getSyspath() {return udev_device_get_syspath(device);}
    const char *getSysname() {return udev_device_get_sysname(device);}
    const char *getSysnum() {return udev_device_get_sysnum(device);}
    const char *getDevnode() {return udev_device_get_devnode(device);}

    struct udev_list_entry *getDevlinksListEntry() {
        return udev_device_get_devlinks_list_entry(device);}
    struct udev_list_entry *getPropertiesListEntry() {
        return udev_device_get_properties_list_entry(device);}
    struct udev_list_entry *getTagsListEntry() {
        return udev_device_get_tags_list_entry(device);}
    const char *getPropertyValue(const char *key) {
        return udev_device_get_property_value(device, key);}
    const char *getDriver() {
        return udev_device_get_driver(device);}
    dev_t getDevnum() {
        return udev_device_get_devnum(device);}
    const char *getAction() {
        return udev_device_get_action(device);}
    unsigned long long int getSeqnum() {
        return udev_device_get_seqnum(device);}
    const char *getSysattrValue(const char *sysattr) {
        return udev_device_get_sysattr_value(device, sysattr);}

    UdevDevice *getParent() {
        return getDevice(udev_device_get_parent(device));}
    UdevDevice *getParentWithSubsystemDevtype(const char *subsystem,
                                              const char *devtype) {
        return getDevice(
                    udev_device_get_parent_with_subsystem_devtype(device,
                                                                  subsystem,
                                                                  devtype));}

    static UdevDevice *getDevice (struct udev_device *device);

    static UdevDevice *getDevice (struct udev *udev, const char *path)
    {
        return UdevDevice::getDevice(udev_device_new_from_syspath(udev, path));
    }

    static void cleanDevicesList (struct udev *udev);
private:
    UdevDevice(struct udev_device *device);
    struct udev *udev() {return udev_device_get_udev(device);}

    struct udev_device *device;
    static QHash<struct udev_device *, UdevDevice *> devices;
};

#endif // UDEVDEVICE_H
