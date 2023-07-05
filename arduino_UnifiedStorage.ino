#include "src/UnifiedStorage.h"


Directory usbRoot;
Directory internalRoot;

void setup(){
    Serial.begin(115200);
    while (!Serial) ;

    internalStorage.setQSPIPartition(2);
    internalStorage.setQSPIPartitionName("user");

      // Initialize the storage
    if (!internalStorage.begin()) {
      Serial.println("Failed to initialize internal storage");
      return;
    }

     if (!usbStorage.begin()) {
      Serial.println("Failed to initialize storage");
      return;
    }

  internalRoot = internalStorage.getRootFolder();
  usbRoot = usbStorage.getRootFolder();
}


void loop(){

}
