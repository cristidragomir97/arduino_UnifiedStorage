#include "USBStorage.h"

#if defined(ARDUINO_PORTENTA_H7_M7)
    #include <Arduino_USBHostMbed5.h>
#endif

#define MAX_TRIES 5

USBStorage::USBStorage(){
}

int USBStorage::begin(){
    int attempts = 0;
    int err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);

    while (0 != err && attempts < MAX_TRIES) {
        attempts +=1;
        err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);
        Serial.println("USB not connected yet, retrying..");
        delay(100);
    }

    if(err == 0){
        this -> connected = true;
    }

    return err == 0;
}

int USBStorage::unmount(){
  auto unmountResult = umount(DEV_USB);
  if(unmountResult == 0){
      this -> connected = false;
  }
  return unmountResult;
}

Directory USBStorage::getRootFolder(){
    return Directory("/usb");
}


bool USBStorage::isAvailable(){
    return this -> available;
}

bool USBStorage::isConnected(){
    return this -> connected;
}errno

void USBStorage::checkConnection(){
    #if defined(ARDUINO_PORTENTA_H7_M7)
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
    #endif
}