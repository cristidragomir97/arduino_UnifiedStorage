#include "Doc.h"


Doc::Doc() {
   this->fp = nullptr;
}

Doc::Doc(const char* path){
    this -> path = (char *)path;
}

bool Doc::changeMode(FileMode mode){
    this->close();
    return this -> open(this->path, mode);
}


bool Doc::open(const char* filename, FileMode fileMode) {
    const char* mode;
    
    this -> path = (char *)filename;
    // Set the mode based on the fileMode
    switch (fileMode) {
        case READ:
            mode = "r+";
            break;
        case WRITE:
            mode = "w+";
            break;
        case APPEND:
            mode = "a+";
            break;
        default:
            mode = "r"; // Default mode is "read"
            break;
    }

    // Open the file
    this->fp = fopen(filename, mode);
    this->fm = fileMode;


    if (this->fp == nullptr) {
        // Failed to open the file
        return false;
    }

    return true;
}

bool Doc::open(String filename, FileMode mode){
    return this -> open(filename.c_str(), mode);
}

void Doc::close() {
    // Close the file
    if (this->fp != nullptr) {
        fclose(this->fp);
        this->fp = nullptr;
    }
}

bool Doc::seek(size_t offset) {
    // Seek to a specific position in the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return false;
    }

    int result = fseek(this->fp, offset, SEEK_SET);
    return (result == 0);
}

int Doc::available() {
    // Check the available data in the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

    int currentPosition = ftell(this->fp);
    fseek(this->fp, 0, SEEK_END);
    int fileSize = ftell(this->fp);
    fseek(this->fp, currentPosition, SEEK_SET);

    return (fileSize - currentPosition);
}

int Doc::read() {
    // Read a single byte from the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

    int value = fgetc(this->fp);
    return value;
}



size_t Doc::read(uint8_t* buffer, size_t size) {
    // Read data from the file into the buffer
    if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

    size_t bytesRead = fread(buffer, sizeof(uint8_t), size, this->fp);
    return bytesRead;
}

String Doc::readAsString() {
  if (this->fp == nullptr) {
    return String("");
  }

  this->seek(0);
  size_t bytesAvailable = this->available();
  uint8_t buffer[bytesAvailable + 1];
  this->read(buffer, bytesAvailable);


  String result = "";
  for (size_t i = 0; i < bytesAvailable; i++) {
    
    result += static_cast<char>(buffer[i]);

  }

  return result;
}

size_t Doc::write(uint8_t value) {
    // Write a single byte to the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

    int result = fputc(value, this->fp);
    return (result != EOF) ? 1 : 0;
}

size_t Doc::write(String data) {

    if (this->fp == nullptr) {
        // File pointer is not valid

        return 0;
    }

    // Convert the String to a C-style null-terminated string
    const char* str = data.c_str();

    // Calculate the length of the string
    size_t dataSize = strlen(str);

    // Write data to the file
    size_t bytesWritten = fwrite(str, sizeof(char), dataSize, this->fp);

    return bytesWritten;
}


size_t Doc::write(const uint8_t* buffer, size_t size){
  if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

     size_t bytesWritten = fwrite(buffer, sizeof(char), size, this->fp);

    return bytesWritten;
    
}


bool Doc::remove() {
    // Remove the file
    int result = ::remove((const char *)this->path);
    if (result == 0) {
        return true;
    } else {
        // Error occurred while removing the file
        return false;
    }
}

bool Doc::rename(const char* newFilename) {
    // Rename the file
    int result = ::rename((const char *)this -> path, newFilename);
    if (result == 0) {
        // Update the internal filename
        this -> path = (char *)newFilename;
        return true;
    } else {
        // Error occurred while renaming the file
        return false;
    }
}

bool Doc::rename(String newFilename){
    return this -> rename(newFilename.c_str());
}

bool Doc::exists() {

    // Check if the file exists
    FILE* file = fopen(this ->path, "r");
    if (file != nullptr) {
        // File exists
        fclose(file);
        return true;
    } else {
        // File does not exist
        return false;
    }
}


bool Doc::copyTo(String destinationPath){
    return this -> copyTo(destinationPath.c_str());
}

bool Doc::copyTo(Directory destinationFolder){
    const char * destinationPath = destinationFolder.getPath();
    this -> copyTo(destinationPath);
}

bool Doc::copyTo(const char* destinationPath) {
 
  const char* newPath =  replaceFirstPathComponent(this-> path, destinationPath);
  
  // Open the source file for reading
  FILE* sourceFile = fopen(this->path, "r");


  if (sourceFile == nullptr) {
    //Serial.println("Failed to open the source file for reading");
    return false;
  }

  // Open the destination file for writing
  FILE* destinationFile = fopen(newPath, "w");
  if (destinationFile == nullptr) {
    //Serial.println("Failed to open the destination file for writing");
    fclose(sourceFile);
    return false;
  }

  // Copy data from the source file to the destination file
  int c;
  while ((c = fgetc(sourceFile)) != EOF) {
    fputc(c, destinationFile);
  }

  // Close both files
  fclose(sourceFile);
  fclose(destinationFile);

  return true;
}


bool Doc::moveTo(String destinationPath){
    return this -> moveTo(destinationPath.c_str());
}

bool Doc::moveTo(Directory destinationFolder){
    const char * destinationPath = destinationFolder.getPath();
    this -> moveTo(destinationPath);
}


bool Doc::moveTo(const char* destinationPath) {
    const char* newPath =  replaceFirstPathComponent(this-> path, destinationPath);
    fclose(this -> fp);
    if (!copyTo(destinationPath)) {
        return false;  // Return false if the copy operation fails
    }

    // Delete the source file
    if (::remove(this->path)) {
        return false;
    }

    this -> open(newPath, this->fm);

    return true;
}

Directory Doc::getParentFolder() {
  // Get the parent folder path
  char* parentPath = (char*)malloc(strlen(this->path) + 1);
    if (parentPath != nullptr) {
    strcpy(parentPath, this->path);
    }
  char* lastSlash = strrchr(parentPath, '/');
  if (lastSlash != nullptr) {
    *lastSlash = '\0'; // Null-terminate the parent folder path
    return Directory(parentPath);
  } else {
    free(parentPath);
    return Directory();
  }
}

const char * Doc::getPath(){
    return this ->path;
}

String Doc::getPathString(){
    return String(this -> path);
}