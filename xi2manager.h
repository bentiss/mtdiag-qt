#ifndef XI2MANAGER_H
#define XI2MANAGER_H
#include "xi2device.h"
#include <map>

class XI2Manager
{
public:
    XI2Manager(x11::Display *dpy);
    void appendXI2Devices(const char *name);

    void dettachDevices ();
    void reattachDevices ();

private:
    x11::Display *dpy;
    std::map<int, XI2Device *> map;
};

#endif // XI2MANAGER_H
