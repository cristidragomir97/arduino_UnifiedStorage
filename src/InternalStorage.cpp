#include "InternalStorage.h"

InternalStorage::InternalStorage(){

}

int InternalStorage::begin(){
   #if defined(ARDUINO_PORTENTA_C33)
            BlockDevice* block_device = BlockDevice::get_default_instance();
            this -> user_data = new MBRBlockDevice(block_device, 2);
            this -> user_data_fs = new FATFileSystem(this->partName);
            int err = this -> user_data_fs -> mount(user_data);
        #elif defined(ARDUINO_PORTENTA_H7_M7) 
            this -> user_data = new mbed::MBRBlockDevice(QSPIFBlockDevice::get_default_instance(), this->partNo);
            this -> user_data_fs =  new mbed::FATFileSystem(this->partName);
            int err = this -> user_data_fs -> mount(this -> user_data);
            return err;
    #endif
}

int InternalStorage::unmount(){

}

Directory InternalStorage::getRootFolder(){
 return Directory("/user");
}


void InternalStorage::setQSPIPartition(int partition){
    this -> partNo = partition;
}

void InternalStorage::setQSPIPartitionName(const char * name){
    this -> partName = (char *)name;
}

int InternalStorage::reformatQSPIPartition(){
    return this -> user_data_fs -> reformat(this -> user_data);
}



 