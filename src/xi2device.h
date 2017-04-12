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

#ifndef XI2DEVICE_H
#define XI2DEVICE_H

#include <string>

struct mt_XDisplay;
struct mt_XIDeviceInfo;

class XI2Device
{
public:
    XI2Device(mt_XDisplay* dpy, mt_XIDeviceInfo *device);
    ~XI2Device();

    int dettach ();
    int change_attachment (int master = 0);

private:
    mt_XDisplay* dpy;
    int deviceID;
    int attachment;
    int previousMaster;
    const std::string name;
};

#endif // XI2DEVICE_H
