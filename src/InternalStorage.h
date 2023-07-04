#include "POSIXStorage.h"
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

class InternalStorage: {
    public:
        InternalStorage();
        // Override begin() method for SD card initialization
        int begin();

        int unmount();

        Directory getRootFolder();

        void setQSPIPartition(int partition);

        void setQSPIPartitionName(const char * name);

        int reformatQSPIPartition();

    private:
        #if defined(ARDUINO_PORTENTA_C33)
        MBRBlockDevice * user_data;
        FATFileSystem * user_data_fs;
        #elif defined(ARDUINO_PORTENTA_H7_M7) 
        mbed::MBRBlockDevice * user_data;
        mbed::FATFileSystem * user_data_fs;
        #endif

        int partNo = 2;
        char * partName = "user";
};

