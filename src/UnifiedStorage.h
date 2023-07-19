#ifndef UnifiedStorage_H
#define UnifiedStorage_H

#if defined(ARDUINO_PORTENTA_C33)
#include "Arduino.h"
#include "POSIXStorage.h"
#include "unistd.h"
#endif

#include "Types.h"
#include "Doc.h"
#include "Directory.h"



    #if defined(ARDUINO_PORTENTA_C33)
        #include "QSPIFlashBlockDevice.h"
        #include <BlockDevice.h>
        #include <MBRBlockDevice.h>
        #include "FATFileSystem.h"
    #elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) || defined(ARDUINO_OPTA)
        #include "QSPIFBlockDevice.h"
        #include "MBRBlockDevice.h"
        #include "FATFileSystem.h"
    #endif 

class UnifiedStorage {
    public:
        // Override begin() method for SD card initialization
        virtual int begin();

        virtual int unmount();

        virtual Directory getRootFolder();
};


#if defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_PORTENTA_H7_M7)
    #include "USBStorage.h"
    #include "SDStorage.h"
    #include "InternalStorage.h"
#elif defined(ARDUINO_OPTA)
    #include "USBStorage.h"
    #include "InternalStorage.h"
#endif

#endif