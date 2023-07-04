#include "File.h"
#include "Directory.h"

File::File() {
    this->fp = nullptr;
}

bool File::open(const char* filename, FileMode fileMode) {
    const char* mode;
    
    this -> path = (char *)filename;
    // Set the mode based on the fileMode
    switch (fileMode) {
        case READ:
            mode = "r";
            break;
        case WRITE:
            mode = "w";
            break;
        case APPEND:
            mode = "a";
            break;
        default:
            mode = "r"; // Default mode is "read"
            break;
    }

    // Open the file
    this->fp = fopen(filename, mode);
    Serial.println(filename);
    Serial.println(mode);

    if (this->fp == nullptr) {
        Serial.println(errno);
        // Failed to open the file
        return false;
    }

    return true;
}

void File::close() {
    // Close the file
    if (this->fp != nullptr) {
        fclose(this->fp);
        this->fp = nullptr;
    }
}

bool File::seek(size_t offset) {
    // Seek to a specific position in the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return false;
    }

    int result = fseek(this->fp, offset, SEEK_SET);
    return (result == 0);
}

int File::available() {
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

int File::read() {
    // Read a single byte from the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return -1;
    }

    int value = fgetc(this->fp);
    return value;
}

size_t File::read(uint8_t* buffer, size_t size) {
    // Read data from the file into the buffer
    if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

    size_t bytesRead = fread(buffer, sizeof(uint8_t), size, this->fp);
    return bytesRead;
}

size_t File::write(uint8_t value) {
    // Write a single byte to the file
    if (this->fp == nullptr) {
        // File pointer is not valid
        return 0;
    }

    int result = fputc(value, this->fp);
    return (result != EOF) ? 1 : 0;
}

size_t File::write(const String& data) {
    // Convert the String to a C-style null-terminated string
    const char* str = data.c_str();

    // Calculate the length of the string
    size_t dataSize = strlen(str);

    // Write data to the file
    size_t bytesWritten = fwrite(str, sizeof(char), dataSize, fp);

    return bytesWritten;
}

bool File::remove() {
    // Remove the file
    int result = ::remove((const char *)this->path);
    if (result == 0) {
        return true;
    } else {
        // Error occurred while removing the file
        return false;
    }
}

bool File::rename(const char* newFilename) {
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

bool File::exists() {

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

bool File::copyTo(const char* destinationPath) {
  // Open the source file for reading
  FILE* sourceFile = fopen(this->path, "r");
  if (sourceFile == nullptr) {
    Serial.println("Failed to open the source file for reading");
    return false;
  }

  // Open the destination file for writing
  FILE* destinationFile = fopen(destinationPath, "w");
  if (destinationFile == nullptr) {
    Serial.println("Failed to open the destination file for writing");
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

bool File::moveTo(const char* destinationPath) {
  // Rename the file to the destination path
  if (rename(this->path, destinationPath) == 0) {
    // Update the internal path
    free(this->path);
    this->path = strdup(destinationPath);
    return true;
  } else {
    Serial.println("Failed to move the file");
    return false;
  }
}

Directory File::getParentFolder() {
  // Get the parent folder path
  char* parentPath = strdup(this->path);
  char* lastSlash = strrchr(parentPath, '/');
  if (lastSlash != nullptr) {
    *lastSlash = '\0'; // Null-terminate the parent folder path
    return Directory(parentPath);
  } else {
    free(parentPath);
    return Directory("/");
}

