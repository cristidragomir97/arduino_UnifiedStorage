#include "POSIXStorage.h"

#include "Directory.h"

class SDStorage {
    public:
        SDStorage();
        // Override begin() method for SD card initialization
        int begin();

        int unmount();

        Directory getRootFolder();
};

