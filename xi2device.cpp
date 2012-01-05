/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the XInput 2 handling
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

#include "xi2device.h"
#include <iostream>

using namespace x11;

XI2Device::XI2Device(Display* dpy, XIDeviceInfo *device):
    dpy(dpy),
    deviceID(device->deviceid),
    attachment(device->attachment),
    previousMaster(-1),
    name(device->name)
{
}

XI2Device::~XI2Device()
{

}

int XI2Device::dettach ()
{
    x11::XIDetachSlaveInfo c;
    int ret;

    if (!attachment)
        return 0;

    previousMaster = attachment;

    c.type = XIDetachSlave;
    c.deviceid = deviceID;

    ret = XIChangeHierarchy(dpy, (XIAnyHierarchyChangeInfo*)&c, 1);
    XSync(dpy, false);
    return ret;
}

int XI2Device::change_attachment (int master)
{
    XIAttachSlaveInfo c;
    int ret;

    if (!master)
        master = previousMaster;

    c.type = XIAttachSlave;
    c.deviceid = deviceID;
    c.new_master = master;

    ret = XIChangeHierarchy(dpy, (XIAnyHierarchyChangeInfo*)&c, 1);
    XSync(dpy, false);
    return ret;
}
