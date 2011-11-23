#include "xi2manager.h"
#include <string.h>
#include <iostream>

using namespace x11;

XI2Manager::XI2Manager(Display *dpy):
    dpy(dpy),
    map(std::map<int, XI2Device *>())
{
}

XI2Manager::~XI2Manager()
{
    std::map<int,XI2Device *>::iterator it;

    for(it = map.begin(); it != map.end(); it++) {
        delete it->second;
    }
}

void XI2Manager::appendXI2Devices(const char *c_name)
{
    int n = 0;
    XIDeviceInfo *infos;
    std::string name = c_name;

    infos = XIQueryDevice(dpy, XIAllDevices, &n);
    for (int i = 0; i < n; ++i) {
        XIDeviceInfo *info = &infos[i];
        if (!name.compare(info->name) && !map.count(info->deviceid)) {
            map[info->deviceid] = new XI2Device (dpy, info);
        }
    }
    XIFreeDeviceInfo(infos);
}

void XI2Manager::dettachDevices ()
{
    std::map<int,XI2Device *>::iterator it;

    for(it = map.begin(); it != map.end(); it++) {
        it->second->dettach();
    }
}

void XI2Manager::reattachDevices ()
{
    std::map<int,XI2Device *>::iterator it;

    for(it = map.begin(); it != map.end(); it++) {
        it->second->change_attachment();
    }
}
