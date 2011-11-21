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
