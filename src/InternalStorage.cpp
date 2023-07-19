#include "UnifiedStorage.h"

InternalStorage::InternalStorage(){

}

int InternalStorage::begin(){
   #if defined(ARDUINO_PORTENTA_C33)
            this -> blockDevice = BlockDevice::get_default_instance();
            this -> userData = new MBRBlockDevice(this->blockDevice, 2);
            this -> userDataFileSystem = new FATFileSystem(this->partitionName);
            int err = this -> userDataFileSystem -> mount(userData);
        #elif defined(ARDUINO_PORTENTA_H7_M7) ||  defined(ARDUINO_OPTA) 
            this -> blockDevice = QSPIFBlockDevice::get_default_instance();
            this -> userData = new mbed::MBRBlockDevice(this->blockDevice, this->partitionNumber);
            this -> userDataFileSystem =  new mbed::FATFileSystem(this->partitionName);
            int err = this -> userDataFileSystem -> mount(this -> userData);
            return err;
    #endif
}

int InternalStorage::unmount(){

}

Directory InternalStorage::getRootFolder(){
 return Directory("/user");
}


void InternalStorage::setQSPIPartition(int partition){
    this -> partitionNumber = partition;
}

void InternalStorage::setQSPIPartitionName(const char * name){
    this -> partitionName = (char *)name;
}

int InternalStorage::reformatQSPIPartition(){
    this -> begin();
    return this -> userDataFileSystem -> reformat(this -> userData);
 
}

#if defined(ARDUINO_PORTENTA_C33)
BlockDevice * InternalStorage::getBlockDevice(){
    return this -> blockDevice;
}
#endif


#if defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_OPTA) 
              mbed::BlockDevice *  InternalStorage::getBlockDevice(){
                return this -> blockDevice;
              }
 
#endif