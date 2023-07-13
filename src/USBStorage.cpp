#include "UnifiedStorage.h"
#include <Arduino_USBHostMbed5.h>





USBStorage::USBStorage(){

}

int USBStorage::begin(){
    int err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);

    while (0 != err) {
        err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);
        delay(100);
    }
    
    return err;
}

int USBStorage::unmount(){
  return umount(DEV_SDCARD);
}

Directory USBStorage::getRootFolder(){
    return Directory("/usb");
}

bool USBStorage::isConnected(){
    USBHostMSD msd2;
    return msd2.connect();
}