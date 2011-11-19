#ifndef KDEVICE_H
#define KDEVICE_H


extern "C" {
#include <limits.h>
#include <linux/input.h>
}

//#ifndef ABS_MT_SLOT
//#define ABS_MT_SLOT 0x2f
//#endif

#define LONG_BITS (sizeof(long) * 8)
/* Number of longs needed to hold the given number of bits */
#define NLONGS(x) (((x) + LONG_BITS - 1) / LONG_BITS)

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
    bool keys[KEY_CNT];
    int rel[REL_CNT];

};

#endif // KDEVICE_H
