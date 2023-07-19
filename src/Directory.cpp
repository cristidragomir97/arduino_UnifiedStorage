#include "Directory.h"



Directory::Directory(){

}

Directory::Directory(const char * path){
  DIR* dir = opendir(path);
  if (dir != nullptr) {
    this -> path = (char *) path;
    closedir(dir);
  } else {
    int result = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    this -> path = (char *) path;
  }
}

Directory::Directory(String dirname){
  Directory(dirname.c_str());
}

Doc Directory::createFile(const char * fileName, FileMode fmode){
  Doc thisFile;
  char* filePath = (char*)malloc(strlen(this->path) + strlen(fileName) + 2);
  strcpy(filePath, this->path);
  strcat(filePath, "/");
  strcat(filePath, fileName);
  thisFile.open(filePath, fmode);
  return thisFile;
}

Doc Directory::createFile(String fileName, FileMode fmode){
 return this ->createFile(fileName.c_str(), fmode);
}

bool Directory::remove() {
  // Remove all files in the directory
  std::vector<Doc> files = this->getFiles();
  for (Doc file : files) {
    file.remove();
  }

  // Remove all subfolders in the directory
  std::vector<Directory> folders = this->getFolders();
  for (Directory directory : folders) {
    directory.remove();
  }

  // Remove the current directory
  if (::remove(this->path) == 0) {
    return true;
  } else {
    // Error occurred while removing the directory
    return false;
  }
}

bool Directory::rename(const char* newDirname) {
    // Rename the directory
    this -> dirname = (char *)newDirname;
    char * newPath = replaceLastPathComponent(this->path, newDirname);


    // actually perform the POSIX command to rename the folder
    int result = ::rename(this -> path,  newPath);
    if (result == 0) {
        // Update the internal directory name
        this -> path = (char *)newPath;
        return true;
    } else {
        // Error occurred while renaming the directory
        return false;
    }
}

bool Directory::rename(String newDirname){
   return this->rename(newDirname.c_str());
}

bool Directory::exists() {
    // Check if the directory exists
    DIR* dir = opendir(this -> path);
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

String Directory::getPathString(){
    return String(this->getPath());
}

Directory Directory::createSubfolder(const char* subfolderName) {
  // Construct the full path of the subfolder
  char* subfolderPath = (char*)malloc(strlen(this->path) + strlen(subfolderName) + 2);
  strcpy(subfolderPath, this->path);
  strcat(subfolderPath, "/");
  strcat(subfolderPath, subfolderName);


  // Create the subfolder
  mkdir(subfolderPath, 0777);
  //free(subfolderPath);

  // Return the subfolder object
  return Directory(subfolderPath);
}

Directory Directory::createSubfolder(String subfolderName){
  return this-> createSubfolder(subfolderName.c_str());
}

std::vector<Doc>  Directory::getFiles() {
  std::vector<Doc> ret;
  DIR* directory = opendir(this->path);
  if (directory != nullptr) {
    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
      if (entry->d_type == DT_REG) { // Regular file
        char* filePath = (char*)malloc(strlen(this->path) + strlen(entry->d_name) + 2);
        strcpy(filePath, this->path);
        strcat(filePath, "/");
        strcat(filePath, entry->d_name);
        ret.push_back(Doc(filePath));
      }
    }
    closedir(directory);
    return ret;
  } else {
    std::vector<Doc> ();
  }
}

std::vector<Directory> Directory::getFolders() {
  std::vector<Directory> ret;

  DIR* directory = opendir(this->path);
  if (directory != nullptr) {
    struct dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
      if (entry->d_type == DT_DIR) { // Directory
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
          char* subfolderPath = (char*)malloc(strlen(this->path) + strlen(entry->d_name) + 2);
          strcpy(subfolderPath, this->path);
          strcat(subfolderPath, "/");
          strcat(subfolderPath, entry->d_name);
          ret.push_back((subfolderPath));
        }
      }
    }
    closedir(directory);
    return ret;
  } else {
    return std::vector<Directory> ();
  }
}

#define PATH_MAX 250

bool Directory::copyTo(const char* destinationPath) {
    const char * source = this -> path;

    const char* fileName = getLastPathComponent((const char *) this->path);
    char* destination = (char*)malloc(strlen(destinationPath) + strlen(fileName) + 2);
    strcpy(destination, destinationPath);
    strcat(destination, "/");
    strcat(destination, fileName);
    
    DIR* dir = opendir(source);
    if (dir == nullptr) {
        printf("Failed to open source directory\n");
        return false;
    }

    // Create destination directory if it doesn't exist
    if (mkdir(destination, 0777) != 0 && errno != EEXIST) {
        printf("Failed to create destination directory\n");
        closedir(dir);
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char sourcePath[PATH_MAX];
            snprintf(sourcePath, PATH_MAX, "%s/%s", source, entry->d_name);

            char destinationPath[PATH_MAX];
            snprintf(destinationPath, PATH_MAX, "%s/%s", destination, entry->d_name);

            struct stat fileInfo;
            if (stat(sourcePath, &fileInfo) != 0) {
                printf("Failed to get file information\n");
                closedir(dir);
                return false;
            }

            if (S_ISDIR(fileInfo.st_mode)) {
                // Recursively copy subdirectories
                if (!copyDirectory(sourcePath, destinationPath)) {
                    closedir(dir);
                    return false;
                }
            } else {
                // Copy regular files
                FILE* sourceFile = fopen(sourcePath, "r");
                if (sourceFile == nullptr) {
                    printf("Failed to open source file for reading\n");
                    closedir(dir);
                    return false;
                }

                FILE* destinationFile = fopen(destinationPath, "w");
                if (destinationFile == nullptr) {
                    printf("Failed to open destination file for writing\n");
                    fclose(sourceFile);
                    closedir(dir);
                    return false;
                }

                int c;
                while ((c = fgetc(sourceFile)) != EOF) {
                    fputc(c, destinationFile);
                }

                fclose(sourceFile);
                fclose(destinationFile);
            }
        }
    }

    closedir(dir);
    return true;
}

bool Directory::copyTo(String destination){
  return this->copyTo(destination.c_str());
}


bool Directory::copyTo(Directory destination){
  return this->copyTo(destination.getPath());
}

bool Directory::moveTo(const char * destination){
  const char* newPath =  replaceFirstPathComponent(this-> path, destination);

  if (!this->copyTo(destination)) {
        return false;  // Return false if the copy operation fails
  }

  if (this->remove()) {
        return false;
  }

  this->path = (char *)newPath;
}

bool Directory::moveTo(Directory destination){
  return this->moveTo(destination.getPath());
}

bool Directory::moveTo(String destination){
  return this->moveTo(destination.c_str());
}
