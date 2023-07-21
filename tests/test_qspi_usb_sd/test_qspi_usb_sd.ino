#include <Arduino.h>
#include "UnifiedStorage.h"

USBStorage usb = USBStorage();
SDStorage sd = SDStorage();
InternalStorage internal = InternalStorage();

void printDirectoryContents(Directory dir, int indentation = 0) {
  std::vector<Directory> directories = dir.getFolders();
  std::vector<Doc> files = dir.getFiles();

  // Print directories
  for (Directory subdir : directories) {
    for (int i = 0; i < indentation; i++) {
      Serial.print("  ");
    }
    Serial.print("[D] ");
    Serial.println(subdir.getPath());
    printDirectoryContents(subdir, indentation + 1);
  }

  // Print files
  for (Doc file : files) {
    for (int i = 0; i < indentation; i++) {
      Serial.print("  ");
    }
    Serial.print("[F] ");
    Serial.println(file.getPath());
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  runTests(&sd, "SD Storage");
  runTests(&internal, "Internal Storage (QSPI)");
  runTests(&usb, "USB Storage");
}

void loop() {
}

void runTests(UnifiedStorage * storage, String storageType) {
  if (storage->begin()) {
    Serial.println("=== Testing " + storageType + " ===");
    Directory root = storage->getRootFolder();
    
    Serial.println("========= File Tests =========");

    testFileCreationWithOpen(root);
    testFileCreationWithCreate(root);
    testWritingCharBuffer(root);
    testWritingWithString(root);
    testAppending(root);
    testReadingAll(root);
    testSeeking(root);
    testAvailableData(root);
    testCopyingFile(root);
    testMovingFile(root);

    Serial.println("========= FS Contents after File Tests =========");
    printDirectoryContents(root);
    Serial.println("=============================\n");

    Serial.println("========= Folder Tests =========");

    testFolderCreation(root);
    testFolderRenaming(root);
    testListingFilesAndDirectories(root);
    testCopyingFolder(root);
    testMovingFolder(root);

    Serial.println("========= FS Contents after Folder Tests =========");
    printDirectoryContents(root);
    Serial.println("=============================\n");
  } else {
    Serial.println(storageType + " not initialized!");
  }
}


void testFileCreationWithOpen(Directory root) {
  Doc document;
  if (document.open(root.getPathString() + "/test_open.txt", FileMode::WRITE)) {
    Serial.println("\n--- Test creating file using document.open ---");
    Serial.println("Test creating file using document.open - Success");
    document.close();
  } else {
    Serial.println("Test creating file using document.open - Failed. Error: " + String(getErrno()));
  }
  document.remove();
}

void testFileCreationWithCreate(Directory root) {
  Serial.println("\n--- Test creating file using root.createFile ---");
  Doc document = root.createFile("test_create.txt", FileMode::WRITE);
  if (document.exists()) {
    Serial.println("Test creating file using root.createFile - Success");
    document.close();
  } else {
    Serial.println("Test creating file using root.createFile - Failed. Error: " + String(getErrno()));
  }
  document.remove();
}

void testWritingCharBuffer(Directory root) {
  Doc document = root.createFile("test_write.txt", FileMode::WRITE);
  if (document.exists()) {
    Serial.println("\n--- Test writing char * buffer ---");
    size_t bytesWritten = document.write(reinterpret_cast<const uint8_t*>("Hello, World!"), 13);
    Serial.println("Bytes written to file: " + String(bytesWritten));
    document.close();
  } else {
    Serial.println("Test writing char * buffer - Failed. Error: " + String(getErrno()));
  }
  document.remove();
}

void testWritingWithString(Directory root) {
  Doc document = root.createFile("test_write_string.txt", FileMode::WRITE);
  if (document.exists()) {
    Serial.println("\n--- Test writing String ---");
    String data = " This is a test with String data.";
    size_t bytesWritten = document.write(data);
    Serial.println("Bytes written to file (with String): " + String(bytesWritten));
    document.close();
  } else {
    Serial.println("Test writing with String data type - Failed. Error: " + String(getErrno()));
  }
  document.remove();
}

void testAppending(Directory root) {
  Doc document = root.createFile("test_append.txt", FileMode::WRITE);
  if (document.exists()) {
    Serial.println("\n--- Test appending ---");
    String data = " Appending some more data.";
    size_t bytesWritten = document.write(reinterpret_cast<const uint8_t*>(data.c_str()), data.length());
    Serial.println("Bytes written to file (appending): " + String(bytesWritten));
    document.close();
  } else {
    Serial.println("Test appending to the file - Failed. Error: " + String(getErrno()));
  }
  document.remove();
}

void testReadingAll(Directory root) {
  Doc document = root.createFile("test_read.txt", FileMode::WRITE);
  if (document.exists()) {
    document.write(reinterpret_cast<const uint8_t*>("Hello, World!"), 13);
    document.close();

    if (document.open(root.getPathString() + "/test_read.txt", FileMode::READ)) {
      char buffer[document.available()];
      size_t bytesRead = document.read(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer));
      buffer[bytesRead] = '\0'; // Null-terminate the string
      Serial.println("\n--- Test reading everything from the file ---");
      Serial.println("Read from file (test_read.txt): " + String(buffer));
      document.close();
    } else {
      Serial.println("Test reading everything from the file - Failed. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test reading everything from the file - Failed to create file. Error: " + String(getErrno()));
  }
  document.remove();
}

void testSeeking(Directory root) {
  Doc document = root.createFile("test_seek.txt", FileMode::WRITE);
  if (document.exists()) {
    document.write(reinterpret_cast<const uint8_t*>("Hello, World!"), 13);
    document.close();

    if (document.open(root.getPathString() + "/test_seek.txt", FileMode::READ)) {
      Serial.println("\n--- Test seeking file ---");
      document.seek(7);
      char buffer[20];
      size_t bytesRead = document.read(reinterpret_cast<uint8_t*>(buffer), sizeof(buffer));
      buffer[bytesRead] = '\0'; // Null-terminate the string
      Serial.println("Read after seeking: " + String(buffer));
      document.close();
    } else {
      Serial.println("Test seeking in the file - Failed. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test seeking in the file - Failed to create file. Error: " + String(getErrno()));
  }
  document.remove();
}

void testAvailableData(Directory root) {
  Doc document = root.createFile("test_available.txt", FileMode::WRITE);
  if (document.exists()) {
    document.write(reinterpret_cast<const uint8_t*>("Hello, World!"), 13);
    document.close();

    if (document.open(root.getPathString() + "/test_available.txt", FileMode::READ)) {
      Serial.println("\n--- Test available data ---");
      int availableBytes = document.available();
      Serial.println("Available bytes in file (test_available.txt): " + String(availableBytes));
      document.close();
    } else {
      Serial.println("Test checking available data in the file - Failed. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test checking available data in the file - Failed to create file. Error: " + String(getErrno()));
  }
  document.remove();
}

void testCopyingFile(Directory root) {
    // Test copying a file
  Doc sourceFile = root.createFile("test_source_copy.txt", FileMode::WRITE);
  if (sourceFile.exists()) {
    sourceFile.write(reinterpret_cast<const uint8_t*>("Hello, World!"), 13);
    sourceFile.close();

    Directory destinationFolder = root.createSubfolder("test_folder");
    if (destinationFolder.exists()) {
      Doc copiedFile = sourceFile;

      if (copiedFile.exists()) {
        Serial.println("\n--- Test copying a file ---");
        Serial.println("Source file name: " + String(sourceFile.getPathString()));
        Serial.println("Destination file name: " + String(copiedFile.getPathString()));
        if (sourceFile.copyTo(destinationFolder)) {
          Serial.println("File copied successfully!");
          copiedFile.close();
          copiedFile.remove();
        } else {
          Serial.println("File copying failed. Error: " + String(getErrno()));
        }
        sourceFile.remove();
      } else {
        Serial.println("Test copying a file - Failed. Error: " + String(getErrno()));
      }

      destinationFolder.remove();
    } else {
      Serial.println("Test copying a file - Failed to create destination folder. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test copying a file - Failed. Error: " + String(getErrno()));
  }


}

void testMovingFile(Directory root) {
    Doc sourceFileMove = root.createFile("test_source_move.txt", FileMode::WRITE);
  if (sourceFileMove.exists()) {
    sourceFileMove.write(reinterpret_cast<const uint8_t*>("Hello, World!"), 13);
    sourceFileMove.close();

    Directory destinationFolder = root.createSubfolder("test_folder");
    if (destinationFolder.exists()) {
      Doc movedFile = sourceFileMove;
      if (movedFile.exists()) {
        Serial.println("\n--- Test moving a file ---");
        Serial.println("Source file name: " + String(sourceFileMove.getPathString()));
        Serial.println("Destination file name: " + String(destinationFolder.getPathString()) + "/test_source_move.txt");
        if (sourceFileMove.moveTo(destinationFolder)) {
          Serial.println("File moved successfully!");
          movedFile.close();
          movedFile.remove();
        } else {
          Serial.println("File moving failed. Error: " + String(getErrno()));
        }
        sourceFileMove.remove();
      } else {
        Serial.println("Test moving a file - Failed. Error: " + String(getErrno()));
      }
    } else {
      Serial.println("Test moving a file - Failed to create destination folder. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test moving a file - Failed. Error: " + String(getErrno()));
  }
}

void testFolderCreation(Directory root) {
  Directory subfolder = root.createSubfolder("test_folder");
  if (subfolder.exists()) {
    Serial.println("\n--- Test creating folder using root.createSubfolder ---");
    Serial.println("Test creating folder using root.createSubfolder - Success");
    subfolder.remove();
  } else {
    Serial.println("Test creating folder using root.createSubfolder - Failed. Error: " + String(getErrno()));
  }
}

void testFolderRenaming(Directory root) {
  Directory sourceFolder = root.createSubfolder("source_folder");
  if (sourceFolder.exists()) {
    Serial.println("\n--- Test renaming folder ---");
    Serial.println("Source folder name: " + String(sourceFolder.getPathString()));
    if (sourceFolder.rename("renamed_folder")) {
      Serial.println("Folder renamed to: " + String(sourceFolder.getPathString()));
      sourceFolder.remove();
    } else {
      Serial.println("Folder renaming failed. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test folder renaming - Failed. Error: " + String(getErrno()));
  }
}

void testListingFilesAndDirectories(Directory root) {
  Serial.println("\n--- Test listing files and directories in the root folder ---");
  std::vector<Doc> rootFiles = root.getFiles();
  Serial.println("Root Files:");
  for (Doc file : rootFiles) {
    Serial.println(" - " + String(file.getPathString()));
  }

  std::vector<Directory> rootFolders = root.getFolders();
  Serial.println("Root Folders:");
  for (Directory folder : rootFolders) {
    Serial.println(" - " + String(folder.getPathString()));
  }
}

void testCopyingFolder(Directory root) {
  Directory sourceFolder = root.createSubfolder("folder1");
  Directory copyDestination = root.createSubfolder("copy_destination");

  if (sourceFolder.exists()) {
    Serial.println("\n--- Test copying a folder ---");
    Serial.println("Source folder name: " + String(sourceFolder.getPathString()));
    Serial.println("Destination folder name: " + String(copyDestination.getPathString()));
    if (sourceFolder.copyTo(copyDestination)) {
      Serial.println("Folder copied successfully!");
    } else {
      Serial.println("Folder copying failed. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test copying a folder - Failed to create source folder. Error: " + String(getErrno()));
  }
}

void testMovingFolder(Directory root) {
  Directory sourceFolderMove = root.createSubfolder("source_folder_move");
  Directory moveDestination = root.createSubfolder("move_destination");

  if (sourceFolderMove.exists()) {
    Serial.println("\n--- Test moving a folder ---");
    Serial.println("Source folder name: " + String(sourceFolderMove.getPathString()));
    Serial.println("Destination folder name: " + String(moveDestination.getPathString()));
    if (sourceFolderMove.moveTo(moveDestination)) {
      Serial.println("Folder moved successfully!");
    } else {
      Serial.println("Folder moving failed. Error: " + String(getErrno()));
    }
  } else {
    Serial.println("Test moving a folder - Failed to create source folder. Error: " + String(getErrno()));
  }
}

void testFolderTraversal(Directory root) {
  Directory traversalFolder = root.createSubfolder("traversal_folder");
  if (traversalFolder.exists()) {
    Directory nestedFolder = traversalFolder.createSubfolder("nested_folder");
    if (nestedFolder.exists()) {
      nestedFolder.createFile("file_in_nested.txt", FileMode::WRITE);
      Serial.println("\n--- Test folder traversal ---");
      Serial.println("Nested Folders:");
      for (Directory folder : root.getFolders()) {
        Serial.println(" - " + String(folder.getPath()));
      }

      Serial.println("Nested Files:");
      for (Doc file : root.getFiles()) {
        Serial.println(" - " + String(file.getPath()));
      }

      nestedFolder.remove();
    }
    traversalFolder.remove();
  }
}
