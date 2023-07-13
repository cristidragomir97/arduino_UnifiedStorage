#include "src/UnifiedStorage.h"
#include <vector>

InternalStorage internalStorage = InternalStorage();
USBStorage usbStorage = USBStorage();

void printTree(Directory dir, int indentation = 0) {
    if(indentation == 0){
      Serial.println("-----------");
      Serial.println(dir.getPath());
      Serial.println("-----------");
    }

    std::vector<Directory> directories = dir.getFolders();
    for (Directory directory : directories) {
      for (int i = 0; i <= indentation; i++) {
        Serial.print("--");
      }

        const char* path = directory.getPath();
        Serial.println(path);
        printTree(directory, indentation + 1);
    }

    std::vector<Doc> files = dir.getFiles();
    for (Doc file : files) {
         for (int i = 0; i <= indentation; i++) {
        Serial.print("--");
      }
       const char* path = file.getPath();
        Serial.println(path);
    }
}


void setup(){
    Serial.begin(115200);
    while (!Serial) ;

   
    if (!internalStorage.begin() == 0) {
      Serial.println("Failed to initialize internal storage");
      return;
    } else {
      Serial.println("initialised storage");
    }

  Directory usbRoot = internalStorage.getRootFolder();
  printTree(usbRoot);

  Serial.println("creating folder structure...");

  Directory fileWrapper = usbRoot.createSubfolder(String("fw1"));
  Directory fileWrapper2 = usbRoot.createSubfolder(String("fw2"));

  Serial.println("printing folder structure...");
  printTree(usbRoot);

  Serial.println("Writing to file");
  Doc file = fileWrapper.createFile("file.txt", FileMode::WRITE);
  file.write("Im writing a string to a file");
  file.changeMode(FileMode::READ);
  delay(1000);
  Serial.println(file.readAsString());

  fileWrapper.moveTo(fileWrapper2);
  printTree(usbRoot);
}


void loop(){
  Serial.println(usbStorage.isConnected());
  delay(10);
}
