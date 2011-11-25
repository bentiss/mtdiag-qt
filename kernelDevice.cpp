#include "kernelDevice.h"
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TestBit(bit, array) ((array[(bit) / LONG_BITS]) & (1L << ((bit) % LONG_BITS)))

KernelDevice::KernelDevice(const char *path,
                 void (*processEvent)(struct input_event*, void*),
                 void *args):
    initialized(false),
    processEvent(processEvent),
    args(args),
    inputID(0)
{
    this->path = (char *)malloc (strlen(path) + 1);
    memcpy(this->path, path, strlen(path) + 1);
}

bool KernelDevice::init()
{
    if (initialized)
        return true;

    fileDescriptor = open(path, O_RDONLY);
    if (fileDescriptor < 0)
        return false;

    memset(abs_bitmask, 0, sizeof(abs_bitmask));
    memset(key_bitmask, 0, sizeof(key_bitmask));
    memset(rel_bitmask, 0, sizeof(rel_bitmask));
    memset(absinfo, 0, sizeof(absinfo));
    memset(keys, false, sizeof(keys));
    memset(rel, 0, sizeof(rel));
    memset(name, '\0', sizeof(name));


    ioctl (fileDescriptor, EVIOCGBIT (EV_ABS, ABS_CNT), abs_bitmask);
    ioctl (fileDescriptor, EVIOCGBIT (EV_KEY, KEY_CNT), key_bitmask);
    ioctl (fileDescriptor, EVIOCGBIT (EV_REL, REL_CNT), rel_bitmask);

    for (int bit = 0; bit < ABS_CNT; ++bit) {
        if (TestBit (bit, abs_bitmask))
             ioctl (fileDescriptor, EVIOCGABS (bit), &absinfo[bit]);
    }

    ioctl (fileDescriptor, EVIOCGNAME(sizeof(name)), name);
    ioctl (fileDescriptor, EVIOCGID, &inputID);

    initialized = true;
    return initialized;
}

KernelDevice::~KernelDevice ()
{
    if (fileDescriptor >= 0)
        close (fileDescriptor);
    free(path);
}

/* just a magic number to reduce the number of reads */
#define NUM_EVENTS 16

void KernelDevice::event ()
{
    struct input_event ev[NUM_EVENTS];
    int i, len = sizeof(ev);

    while (len == sizeof(ev))
    {
        len = read(fileDescriptor, &ev, sizeof(ev));
        if (len <= 0)
        {
            if (errno == ENODEV) /* May happen after resume */
            {
                close(fileDescriptor);
                fileDescriptor = -1;
            } else if (errno != EAGAIN)
                std::cerr << name << "Read error: " << strerror(errno) << std::endl;
            break;
        }

        /* The kernel promises that we always only read a complete
         * event, so len != sizeof ev is an error. */
        if (len % sizeof(ev[0])) {
            /* We use X_NONE here because it doesn't alloc */
            std::cerr << name << "Read error: " << strerror(errno) << std::endl;
            break;
        }

        for (i = 0; i < (int)(len/sizeof(ev[0])); i++) {
            switch (ev[i].type) {
            case EV_ABS:
                absinfo[ev[i].code].value = ev[i].value;
                break;
            case EV_REL:
                rel[ev[i].code] = ev[i].value;
                break;
            case EV_KEY:
                 keys[ev[i].code] = ev[i].value;
                 break;
            }
            if (processEvent)
                processEvent(&ev[i], args);
        }
    }
}

bool KernelDevice::hasAbs (unsigned int code)
{
    return TestBit (code, abs_bitmask);
}

struct input_absinfo *KernelDevice::getAbsinfo (bool *ok, unsigned int code)
{
    *ok = code <= ABS_MAX;
    return &absinfo[code];
}

bool KernelDevice::hasKey (unsigned int code)
{
    return TestBit (code, key_bitmask);
}

bool KernelDevice::getKey (bool *ok, unsigned int code)
{
    *ok = code <= KEY_MAX;
    return keys[code];
}

bool KernelDevice::hasRel (unsigned int code)
{
    return TestBit (code, rel_bitmask);
}

int KernelDevice::getRel (bool *ok, unsigned int code)
{
    *ok = code <= REL_MAX;
    return rel[code];
}
