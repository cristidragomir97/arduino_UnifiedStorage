#include "POSIXStorage.h"
#include "Directory.h"


class USBStorage {
    public:
        USBStorage();
        // Override begin() method for SD card initialization
        int begin();

        int unmount();

        Directory getRootFolder();
};

