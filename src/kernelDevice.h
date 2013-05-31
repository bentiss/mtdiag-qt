/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the kernel access to a multitouch device
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

#ifndef KDEVICE_H
#define KDEVICE_H


extern "C" {
#include <limits.h>
#include <linux/input.h>

#ifndef ABS_MT_DISTANCE
#define ABS_MT_DISTANCE		0x3b	/* Contact hover distance */
#endif

#ifndef ABS_MT_TOOL_X
#define ABS_MT_TOOL_X		0x3c	/* Center X tool position */
#define ABS_MT_TOOL_Y		0x3d	/* Center Y tool position */
#endif
}

#define LONG_BITS (sizeof(long) * 8)
/* Number of longs needed to hold the given number of bits */
#define NLONGS(x) (((x) + LONG_BITS - 1) / LONG_BITS)

#define MAX_SLOT 250

class KernelDevice
{
public:
    KernelDevice(const char *path, void (*processEvent)(struct input_event*, void*) = 0,
            void *args = 0);
    ~KernelDevice ();

    int getFileDescriptor () {return fileDescriptor;}
    bool isInitialized () { return initialized; }

    void setProcessEventArgs (void (*processEvent)(struct input_event*, void*),
                              void *args)
    {
        this->processEvent = processEvent;
        this->args = args;
    }

    bool init();
    void event ();

    const char *getName() {return name;}
    const char *getPath() {return path;}

    bool hasAbs (unsigned int code);
    struct input_absinfo *getAbsinfo (bool *ok, unsigned int code);

    bool hasKey (unsigned int code);
    bool getKey (bool *ok, unsigned int code);

    bool hasRel (unsigned int code);
    int getRel (bool *ok, unsigned int code);

    int getInitialInRangeState (unsigned int slot);

private:
    char *path;
    bool initialized;
    void (*processEvent)(struct input_event*, void*);
    void *args;
    int fileDescriptor;
    char name[1024];
    int inputID;


    unsigned long key_bitmask[NLONGS(KEY_CNT)];
    unsigned long rel_bitmask[NLONGS(REL_CNT)];
    unsigned long abs_bitmask[NLONGS(ABS_CNT)];
    struct input_absinfo absinfo[ABS_CNT];
    int inRangeStates[MAX_SLOT];
    bool keys[KEY_CNT];
    int rel[REL_CNT];

};

#endif // KDEVICE_H
