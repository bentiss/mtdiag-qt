/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the udev C++ layer
 **
 **	Contributors:
 **		Benjamin Tissoires <benjamin.tissoires@enac.fr>
 **
 **
 **	This program is provided to you as free software;
 **	you can redistribute it	and/or modify it under the terms of the
 **	GNU General Public License as published by the Free Software
 **	Foundation; either version 2 of the License, or (at your option)
 **	any later version.
 **/

#ifndef UDEVDEVICE_H
#define UDEVDEVICE_H

struct udev;
struct udev_device;

class UdevDevice
{
public:
    UdevDevice(struct udev_device *device);
    UdevDevice(struct udev *udev, const char *path);
    ~UdevDevice();

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

private:
    struct udev_device *device;
};

#endif // UDEVDEVICE_H
