#include "UnifiedStorage.h"

#ifndef USBStorage_H
#define USBStorage_H

class USBStorage : public UnifiedStorage {
    public:
        USBStorage();
        // Override begin() method for SD card initialization
        int begin() override;

        int unmount() override;

        Directory getRootFolder() override;

        bool isConnected();
        
        bool isAvailable();

        void checkConnection();
    private:
        bool connected = false;
        bool available = false;
        unsigned long previousMillis; 
        int interval = 500;
};

#endif