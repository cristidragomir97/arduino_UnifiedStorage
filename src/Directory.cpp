#include "Directory.h"
#include "unistd.h"

Directory::Directory(){

}

Directory::Directory(const char * dirname){
    // Create the directory
    int result = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    this -> path = (char *) dirname;

}

bool Directory::remove(const char* dirname) {
  
    // Remove the directory
    int result = rmdir(dirname);
    if (result == 0) {
        return true;
    } else {
        // Error occurred while removing the directory
        return false;
    }
}

bool Directory::rename(const char* newDirname) {
    // Rename the directory
    int result = ::rename(this -> dirname, newDirname);
    if (result == 0) {
        // Update the internal directory name
        this -> dirname = (char *)newDirname;
        return true;
    } else {
        // Error occurred while renaming the directory
        return false;
    }
}

bool Directory::exists() {
    // Check if the directory exists
    DIR* dir = opendir(this -> dirname);
    if (dir != nullptr) {
        // Directory exists
        closedir(dir);
        return true;
    } else {
        // Directory does not exist
        return false;
    }
}

const char * Directory::getPath(){
    return this -> path;
}

Directory Directory::createSubfolder(const char* subfolderName) {
  // Construct the full path of the subfolder
  char* subfolderPath = (char*)malloc(strlen(this->path) + strlen(subfolderName) + 2);
  strcpy(subfolderPath, this->path);
  strcat(subfolderPath, "/");
  strcat(subfolderPath, subfolderName);

  // Create the subfolder
  mkdir(subfolderPath, 0777);
  free(subfolderPath);

  // Return the subfolder object
  return Directory(this->path);
}

bool Directory::removeSubfolder(const char* subfolderName) {
  // Construct the full path of the subfolder
  char* subfolderPath = (char*)malloc(strlen(this->path) + strlen(subfolderName) + 2);
  strcpy(subfolderPath, this->path);
  strcat(subfolderPath, "/");
  strcat(subfolderPath, subfolderName);

  // Remove the subfolder
  if (rmdir(subfolderPath) == 0) {
    free(subfolderPath);
    return true;
  } else {
    Serial.println("Failed to remove the subfolder");
    free(subfolderPath);
    return false;
  }
}

bool Directory::renameSubfolder(const char* oldName, const char* newName) {
  // Construct the full paths of the old and new subfolders
  char* oldPath = (char*)malloc(strlen(this->path) + strlen(oldName) + 2);
  char* newPath = (char*)malloc(strlen(this->path) + strlen(newName) + 2);
  strcpy(oldPath, this->path);
  strcat(oldPath, "/");
  strcat(oldPath, oldName);
  strcpy(newPath, this->path);
  strcat(newPath, "/");
  strcat(newPath, newName);

  // Rename the subfolder
  if (::rename(oldPath, newPath) == 0) {
    free(oldPath);
    free(newPath);
    return true;
  } else {
    Serial.println("Failed to rename the subfolder");
    free(oldPath);
    free(newPath);
    return false;
  }
}

void Directory::listFiles() {
  DIR* directory = opendir(this->path);
  if (directory != nullptr) {
    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
      if (entry->d_type == DT_REG) { // Regular file
        Serial.print("File: ");
        Serial.println(entry->d_name);
      }
    }
    closedir(directory);
  } else {
    Serial.println("Failed to open the directory");
  }
}

void Directory::listSubfolders() {
  DIR* directory = opendir(this->path);
  if (directory != nullptr) {
    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
      if (entry->d_type == DT_DIR) { // Directory
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
          Serial.print("Subfolder: ");
          Serial.println(entry->d_name);
        }
      }
    }
    closedir(directory);
  } else {
    Serial.println("Failed to open the directory");
  }
}

