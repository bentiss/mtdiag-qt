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
