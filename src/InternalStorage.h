

#ifndef InternalStorage_H
#define InternalStorage_H

#include "UnifiedStorage.h"

class InternalStorage : public UnifiedStorage {


    public:
        InternalStorage();
        // Override begin() method for SD card initialization
        int begin() override;

        int unmount() override;

        Directory getRootFolder() override;

        void setQSPIPartition(int partition);

        void setQSPIPartitionName(const char * name);

        int reformatQSPIPartition();
        
        #if defined(ARDUINO_PORTENTA_C33)
            BlockDevice * getBlockDevice();
        #elif defined(ARDUINO_PORTENTA_H7_M7)  ||  defined(ARDUINO_OPTA)
            mbed::BlockDevice  * getBlockDevice();
        #endif


    private:
        #if defined(ARDUINO_PORTENTA_C33)
        BlockDevice * blockDevice;
        MBRBlockDevice * userData;
        FATFileSystem * userDataFileSystem;
        #elif defined(ARDUINO_PORTENTA_H7_M7)  ||  defined(ARDUINO_OPTA)
        mbed::BlockDevice * blockDevice;
        mbed::MBRBlockDevice * userData;
        mbed::FATFileSystem * userDataFileSystem;
        #endif

        int partitionNumber = 2;
        char * partitionName = "user";
};

#endif