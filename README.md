# Unified Storage Library

The UnifiedStorage library provides a unified interface to access different types of storage, including internal storage, SD cards, and USB mass storage devices. It simplifies the handling of files and directories across multiple storage mediums on Portenta, Opta, and some Nicla boards.


## Examples
* **examples/basic** - this example is concerned with reading/writing and seeking
* **examples/advanced** - this example is concerned with more advanced features like creating folders, traversing folder sturctures and moving/copying from one storage medium to another
* **examples/opta_logger** - this is more of a real life usecase, where this library is used on an Arduino Opta to log sensor data to a file on the internal storage and check if a USB Mass Storage deviece is inserted. If it is detected it will backup the information on the internal storage, only copying the bytes that are new since the last update.
  
## Features

### Use QSPI Flash, SD cards, and USB mass storage devices
This library allows you to easily switch between different storage mediums on supported boards. See the "Compatibility" section for the list of supported storage mediums.

To initialize the storage medium, create a `UnifiedStorage` object and call its `begin()` method.

```cpp
UnifiedStorage storageMedium = USBStorage();
storageMedium.begin();
```

### Open, Write, and Read Files
 
#### Creating / Opening / Closing Files

You can create or open a file by providing a reference to the root folder of the storage medium or by specifying the full absolute path.

```cpp
Directory root = storageMedium.getRootFolder();
Doc document = root.createFile("file.txt", FileMode::READ);
```

```cpp
Doc document = Doc();
document.open("<absolute_path>/file.txt", FileMode::READ);
```

You can change the mode of an opened file using the `changeMode()` method.

```cpp
document.changeMode(FileMode::WRITE);
```

#### Reading and Writing Files

You can read data from a file using the `read()` method and write data to a file using the `write()` method.

```cpp
uint8_t buffer[128];
size_t bytesRead = document.read(buffer, sizeof(buffer));
```

```cpp
String data = "Hello, World!";
char buffer[data.length() + 1];
data.toCharArray(buffer, sizeof(buffer));
size_t bytesWritten = document.write(reinterpret_cast<const uint8_t*>(buffer), data.length());
```

Alternatively, you can read and write files using strings.

```cpp
String content = document.readAsString();
```

```cpp
String data = "Hello, World!";
size_t bytesWritten = document.write(data);
```

### File Seeking and Availability

The `seek()` and `available()` methods provide functionality for file seeking and checking the available data in a file. Here's an overview of how to use these methods:

#### Seek to a Specific Position in the File

The `seek()` method allows you to move the file pointer to a specific position in the file. It takes an offset parameter, which represents the number of bytes to move from a reference position. Here's an example:

```cpp
// Seek to the 10th byte in the file
bool success = file.seek(10);
if (success) {
  // File pointer is now at the desired position
} else {
  // Seek operation failed
}
```

In this example, the `seek()` method is called with an offset of 10, which moves the file pointer to the 10th byte in the file. The method returns a boolean value indicating the success of the seek operation.

#### Check Available Data in the File

The `available()` method returns the number of bytes available for reading from the current file position. It can be used to determine how much data is left to be read in the file. Here's an example:

```cpp
// Check the available data in the file
int availableBytes = file.available();
if (availableBytes > 0) {
  // There is data available to be read
} else {
  // No more data available
}
```

In this example, the `available()` method is called to retrieve the number of available bytes in the file. If the value is greater than 0, it means there is data available to be read. Otherwise, there is no more data left in the file.

These methods are useful for scenarios where you need to navigate to a specific position in a file or check the availability of data before performing read operations.

### File Manipulation
The library provides various file manipulation operations such as renaming, deleting, moving, and copying files.

#### Renaming a File

You can rename an existing file using the `rename()` method.

```cpp
bool success = document.rename("newfile.txt");
```

#### Deleting a File

You can delete a file using the `remove()` method.

```cpp
bool success = document.remove();
```

#### Moving a File

You can move a file to a different directory using the `moveTo()` method.

```cpp
Directory destination = storageMedium.getRootFolder().createSubfolder("destination");
bool success = document.moveTo(destination);
```

#### Copying a File

You can copy a file to a different directory using the `copyTo()` method.

```cpp
Directory destination = storageMedium.getRootFolder().createSubfolder("destination");
bool success = document.copyTo(destination);
```

### Folder Manipulation
The library provides methods to create, rename, delete, move, copy, and list directories.

#### Creating a Directory

You can create a new directory using the `createSubfolder()` method.

```cpp
Directory root = storageMedium.getRootFolder();
Directory newFolder = root.createSubfolder("new_folder");
```

#### Renaming a Directory

You can rename an existing directory using the `rename()` method.

```cpp
bool success = newFolder.rename("renamed_folder");
```

#### Deleting a Directory

You can delete a directory using the `remove()` method.

```cpp
bool success = newFolder.remove();
```

#### Moving a Directory

You can move a directory to a different location using the `moveTo()` method.

```cpp
Directory destination = storageMedium.getRootFolder().createSubfolder("destination");
bool success = newFolder.moveTo(destination);
```

#### Copying a Directory

You can copy a directory to a different location using the `copyTo()` method.

```cpp
Directory destination = storageMedium.getRootFolder().createSubfolder("destination");
bool success = newFolder.copyTo(destination);
```

#### Listing Files and Directories

You can get a list of files and directories in a directory using the `getFiles()` and `getFolders()` methods.

```cpp
std::vector<Doc> files = root.getFiles();
std::vector<Directory> folders = root.getFolders();
```

#### Traversing Files and Directories

You can traverse through all the files and directories in a directory using a loop and the `getFiles()` and `getFolders()` methods.

```cpp
std::vector<Doc> files = root.getFiles();
for ( Doc file : files) {
    // Perform operations on each file
}

std::vector<Directory> folders = root.getFolders();
for (Directory folder : folders) {
    // Perform operations on each folder
}
```

## Compatibility

This library is compatible with STM32 and Renesas based Arduino boards. The availability of storage mediums depends on the hardware interfaces.

* Portenta Machine Control: USB and Internal QSPI Flash
* Portenta H7 + Portenta Breakout: USB, SD, and QSPI
* Portenta H7 + Vision Shield: SD and QSPI
* Arduino Giga R1: USB and QSPI
* Arduino Opta: USB and QSPI
* Portenta C33 + Portenta Breakout: USB, SD, and QSPI
* Portenta C33 + Vision Shield: SD and QSPI
