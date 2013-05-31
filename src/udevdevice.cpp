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

#include "udevdevice.h"
extern "C" {
#include <libudev.h>
}

UdevDevice::UdevDevice(struct udev_device *device):
    device(device)
{
}

UdevDevice::UdevDevice(struct udev *udev, const char *path)
{
    device = udev_device_new_from_syspath(udev, path);
}

UdevDevice::~UdevDevice()
{
    if (device)
        udev_device_unref(device);
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

UdevDevice *UdevDevice::getParent()
{
    struct udev_device *parent = udev_device_get_parent(device);
    udev_device_ref(parent);
    return new UdevDevice(parent);
}

UdevDevice *UdevDevice::getParentWithSubsystemDevtype(const char *subsystem,
                                          const char *devtype)
{
    struct udev_device *parent = udev_device_get_parent_with_subsystem_devtype(
                device,
                subsystem,
                devtype);
    udev_device_ref(parent);
    return new UdevDevice(parent);
}
