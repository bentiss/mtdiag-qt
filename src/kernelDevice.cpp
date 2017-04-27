/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the kernel access to a multitouch device
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

#include "kernelDevice.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define TestBit(bit, array) ((array[(bit) / LONG_BITS]) & (1L << ((bit) % LONG_BITS)))

struct input_mt_request_layout {
    __u32 code;
    __s32 values[MAX_SLOT];
};

KernelDevice::KernelDevice(const char *path, int fd,
                 void (*processEvent)(struct input_event*, void*),
                 void *args):
    initialized(false),
    processEvent(processEvent),
    args(args),
    fileDescriptor(fd)
{
    this->path = (char *)malloc (strlen(path) + 1);
    memcpy(this->path, path, strlen(path) + 1);
}

bool KernelDevice::init()
{
    if (initialized)
        return true;

    int rc = libevdev_new_from_fd(fileDescriptor, &evdev);
    if (rc < 0) {
        close (fileDescriptor);
        fileDescriptor = -1;
        return false;
    }

    strncpy(name, libevdev_get_name(evdev), sizeof(name));

    initialized = true;
    return initialized;
}

KernelDevice::~KernelDevice ()
{
    if (fileDescriptor >= 0)
        close (fileDescriptor);
    free(path);
}

void KernelDevice::event ()
{
    int rc;
    struct input_event ev;

    do {
        rc = libevdev_next_event (evdev, LIBEVDEV_READ_FLAG_NORMAL, &ev);

        if ((rc == LIBEVDEV_READ_STATUS_SUCCESS || rc == LIBEVDEV_READ_STATUS_SYNC) && processEvent)
            processEvent(&ev, args);

        if (rc == -ENODEV)
            return;
    } while (rc != -EAGAIN);
}

bool KernelDevice::getIndirect()
{
    return !libevdev_has_property(evdev, INPUT_PROP_DIRECT);
}

bool KernelDevice::hasAbs (unsigned int code)
{
    return libevdev_has_event_code(evdev, EV_ABS, code);
}

const struct input_absinfo *KernelDevice::getAbsinfo (bool *ok, unsigned int code)
{
    const struct input_absinfo *absinfo = libevdev_get_abs_info(evdev, code);
    *ok = absinfo != NULL;
    return absinfo;
}

bool KernelDevice::hasKey (unsigned int code)
{
     return libevdev_has_event_code(evdev, EV_KEY, code);
}

bool KernelDevice::getKey (bool *ok, unsigned int code)
{
    int value;
    int rc = libevdev_fetch_event_value	(evdev, EV_KEY, code, &value);
    *ok = rc > 0;
    return value;
}

bool KernelDevice::hasRel (unsigned int code)
{
    return libevdev_has_event_code(evdev, EV_REL, code);
}

int KernelDevice::getRel (bool *ok, unsigned int code)
{
    int value;
    int rc = libevdev_fetch_event_value	(evdev, EV_REL, code, &value);
    *ok = rc > 0;
    return value;
}

int KernelDevice::getInitialInRangeState (unsigned int slot)
{
    int value;
    int rc = libevdev_fetch_slot_value(evdev, slot, ABS_MT_DISTANCE, &value);
    if (!rc)
        return 0;
    return value;
}
