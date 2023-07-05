#include "SDStorage.h"

SDStorage::SDStorage(){

}

int SDStorage::begin(){
  return mount(DEV_SDCARD, FS_FAT, MNT_DEFAULT);
}

int SDStorage::unmount(){
  return umount(DEV_SDCARD);
}

Directory SDStorage::getRootFolder(){
    return Directory("/sdcard");
}