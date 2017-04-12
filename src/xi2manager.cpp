/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the XInput 2 handling
 **
 **	Contributors:
 **		Benjamin Tissoires <benjamin.tissoires@gmail.com>
 **
 **
 **	This program is provided to you as free software;
 **	you can redistribute it	and/or modify it under the terms of the
 **	GNU General Public License as published by the Free Software
 **	Foundation; either version 2 of the License, or (at your option)
 **	any later version.
 **/

#include "xi2manager.h"
#include "x11.h"
#include <string.h>
#include <iostream>

XI2Manager::XI2Manager(mt_XDisplay *_dpy):
    map(std::map<int, XI2Device *>())
{
    if (_dpy == NULL)
        dpy = (mt_XDisplay *)x11::XOpenDisplay(NULL);
    else
        dpy = _dpy;
}

XI2Manager::~XI2Manager()
{
    std::map<int,XI2Device *>::iterator it;

    for(it = map.begin(); it != map.end(); it++) {
        delete it->second;
    }

    x11::XCloseDisplay(mt_to_display(dpy));
}

void XI2Manager::appendXI2Devices(const char *c_name)
{
    int n = 0;
    x11::XIDeviceInfo *infos;
    std::string name = c_name;

    infos = x11::XIQueryDevice(mt_to_display(dpy), XIAllDevices, &n);
    for (int i = 0; i < n; ++i) {
        x11::XIDeviceInfo *info = &infos[i];
        if (!name.compare(info->name) && !map.count(info->deviceid)) {
            map[info->deviceid] = new XI2Device (dpy, (mt_XIDeviceInfo *)info);
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
