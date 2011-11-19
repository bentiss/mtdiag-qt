#ifndef UDEVDEVICE_H
#define UDEVDEVICE_H
#include <QHash>

struct udev;
struct udev_device;

class UdevDevice
{
public:
    const char *getDevpath();
    const char *getSubsystem();
    const char *getDevtype();
    const char *getSyspath();
    const char *getSysname();
    const char *getSysnum();
    const char *getDevnode();

    struct udev_list_entry *getDevlinksListEntry();
    struct udev_list_entry *getPropertiesListEntry();
    struct udev_list_entry *getTagsListEntry();

    const char *getPropertyValue(const char *key);
    const char *getDriver();
    //dev_t getDevnum();
    const char *getAction();
    unsigned long long int getSeqnum();
    const char *getSysattrValue(const char *sysattr);

    UdevDevice *getParent();
    UdevDevice *getParentWithSubsystemDevtype(const char *subsystem,
                                              const char *devtype);

    static UdevDevice *getDevice (struct udev_device *device);

    static UdevDevice *getDevice (struct udev *udev, const char *path);

    static void cleanDevicesList (struct udev *udev);
private:
    UdevDevice(struct udev_device *device);
    struct udev *udev();

    struct udev_device *device;
    static QHash<struct udev_device *, UdevDevice *> devices;
};

#endif // UDEVDEVICE_H
