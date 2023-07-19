#include "USBStorage.h"
#include <Arduino_USBHostMbed5.h>





USBStorage::USBStorage(){
}

int USBStorage::begin(){
    int err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);

    while (0 != err) {
        err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);
        Serial.println(err);
        Serial.println(errno);
        delay(100);
    }

    
    if(err == 0){
        this -> connected = true;
    }

    return err;
}

int USBStorage::unmount(){
  this -> connected = false;
  return umount(DEV_USB);

}

Directory USBStorage::getRootFolder(){
    return Directory("/usb");
}


bool USBStorage::isAvailable(){
    return this -> available;
}

bool USBStorage::isConnected(){
    return this -> connected;
}

void USBStorage::checkConnection(){
    USBHost * host;
    USBDeviceConnected * dev;
    unsigned long currentMillis = millis();
    boolean found = false;

    if (currentMillis - previousMillis >= interval) {
        this -> previousMillis = currentMillis;

            host = USBHost::getHostInst();

                for (uint8_t i = 0; i < MAX_DEVICE_CONNECTED; i++) {
                    if ((dev = host->getDevice(i)) != NULL) {
                        found = true;
                     
                    }
                }
            if(found){
                this->available = true;
            } else {
                this->available = false;
            }
    }
}