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
    #elif defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4)
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

class SDStorage : public UnifiedStorage {
    public:
        SDStorage();
        // Override begin() method for SD card initialization
        int begin() override ;

        int unmount() override;

        Directory getRootFolder() override;
};

class USBStorage : public UnifiedStorage {
    public:
        USBStorage();
        // Override begin() method for SD card initialization
        int begin() override;

        int unmount() override;

        Directory getRootFolder() override;

        bool isConnected();
};




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
        #elif defined(ARDUINO_PORTENTA_H7_M7) 
            mbed::BlockDevice  * getBlockDevice();
        #endif


    private:
        #if defined(ARDUINO_PORTENTA_C33)
        BlockDevice * blockDevice;
        MBRBlockDevice * userData;
        FATFileSystem * userDataFileSystem;
        #elif defined(ARDUINO_PORTENTA_H7_M7) 
        mbed::BlockDevice * blockDevice;
        mbed::MBRBlockDevice * userData;
        mbed::FATFileSystem * userDataFileSystem;
        #endif

        int partitionNumber = 2;
        char * partitionName = "user";
};



