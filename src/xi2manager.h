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

#ifndef XI2MANAGER_H
#define XI2MANAGER_H
#include "xi2device.h"
#include <map>

class XI2Manager
{
public:
    XI2Manager(mt_XDisplay *dpy);
    ~XI2Manager();
    void appendXI2Devices(const char *name);

    void dettachDevices ();
    void reattachDevices ();

private:
    mt_XDisplay *dpy;
    std::map<int, XI2Device *> map;
};

#endif // XI2MANAGER_H
