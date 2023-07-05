#include "USBStorage.h"

USBStorage::USBStorage(){

}

int USBStorage::begin(){
    int err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);
    while (0 != err) {
        int err = mount(DEV_USB, FS_FAT, MNT_DEFAULT);
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