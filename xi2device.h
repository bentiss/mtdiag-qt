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

#ifndef XI2DEVICE_H
#define XI2DEVICE_H

#include <string>

namespace x11 {
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
}

class XI2Device
{
public:
    XI2Device(x11::Display* dpy, x11::XIDeviceInfo *device);
    ~XI2Device();

    int dettach ();
    int change_attachment (int master = 0);

private:
    x11::Display* dpy;
    int deviceID;
    int attachment;
    int previousMaster;
    const std::string name;
};

#endif // XI2DEVICE_H
