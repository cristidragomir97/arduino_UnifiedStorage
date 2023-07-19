#include "UnifiedStorage.h"


// This library allows you to use multiple storage mediums, changing from one to another is as simple as uncommenting these lines"

SDStorage unifiedStorage = SDStorage(); // or
//USBStorage unifiedStorage = USBStorage() // or
//InternalStorage unifiedStorage = InternalStorage();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if(!unifiedStorage.begin()==0){
    Serial.println("error mounting SD Card");
  }
  // Create a root directory
  
  Directory root = unifiedStorage.getRootFolder();

  // Create subdirectories inside the root directory
  Directory subdir1 = root.createSubfolder("subdir1");
  Directory subdir2 = root.createSubfolder("subdir2");
  Directory subdir3 = root.createSubfolder("subdir3");

  // Create files inside the subdirectories
  Doc file1 = subdir1.createFile("file1.txt", FileMode::WRITE);
  Doc file2 = subdir2.createFile("file2.txt", FileMode::WRITE);
  Doc file3 = subdir3.createFile("file3.txt", FileMode::WRITE);

  // Write data to the files
  file1.write("This is file 1.");
  file2.write("This is file 2.");
  file3.write("This is file 3.");

  // Read data from the files using seek and available
  Serial.println("Reading data from files using seek and available:");

  // Close and open files in reading mode
  file1.changeMode(FileMode::READ);
  file2.changeMode(FileMode::READ);
  file3.changeMode(FileMode::READ);


  // Read data from file1
  file1.seek(0); // Move the file pointer to the beginning
  while (file1.available()) {
    char data = file1.read();
    Serial.write(data);
  }
  Serial.println();

  // Read data from file2
  file2.seek(0); // Move the file pointer to the beginning
  while (file2.available()) {
    char data = file2.read();
    Serial.write(data);
  }
  Serial.println();

  // Read data from file3
  file3.seek(0); // Move the file pointer to the beginning
  while (file3.available()) {
    char data = file3.read();
    Serial.write(data);
  }
  Serial.println();
}

void loop() {
  // Nothing to do here
}
