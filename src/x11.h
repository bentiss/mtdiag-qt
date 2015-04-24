#ifndef X11_H
#define X11_H

#include "xi2device.h"

namespace x11 {
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
}

static inline x11::Display *mt_to_display(mt_XDisplay *dpy)
{
    return (x11::Display *)dpy;
}

static inline x11::XIDeviceInfo *mt_to_deviceinfo(mt_XIDeviceInfo *info)
{
    return (x11::XIDeviceInfo *)info;
}

#endif // X11_H

