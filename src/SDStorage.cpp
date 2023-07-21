#include "SDStorage.h"



#if defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_C33)

SDStorage::SDStorage(){

}

int SDStorage::begin(){
  return mount(DEV_SDCARD, FS_FAT, MNT_DEFAULT) == 0;
}

int SDStorage::unmount(){
  return umount(DEV_SDCARD);
}

Directory SDStorage::getRootFolder(){
    return Directory("/sdcard");
}

#endif