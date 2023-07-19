

#ifndef Types_H
#define Types_H
#define PATH_MAX 255

#include "Arduino.h"
#include "POSIXStorage.h"
#include <iostream>
#include <vector>


static bool copyDirectory(const char* source, const char* destination) {
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

static char* replaceLastPathComponent(const char* path, const char* newComponent) {
    char* newPath = nullptr;
    char* lastSlash = const_cast<char*>(strrchr(path, '/'));
    if (lastSlash != nullptr) {
        size_t newPathLength = lastSlash - path + 1 + strlen(newComponent) + 1;
        newPath = static_cast<char*>(malloc(newPathLength));
        strncpy(newPath, path, lastSlash - path + 1);
        newPath[lastSlash - path + 1] = '\0';
        strcat(newPath, newComponent);
    }
    return newPath;
}

static const char* getLastPathComponent(const char* path) {
    const char* lastSlash = strrchr(path, '/');
    if (lastSlash != nullptr) {
        return lastSlash + 1; // Skip the slash to get the last component
    }
    return path; // If no slash is found, the path itself is the last component
}

inline static const char * replaceFirstPathComponent(const char * path, const char * destinationPath){
  const char* fileName = getLastPathComponent((const char *) path);
  char* newPath = (char*)malloc(strlen(destinationPath) + strlen(fileName) + 2);
  strcpy(newPath, destinationPath);
  strcat(newPath, "/");
  strcat(newPath, fileName);

  return newPath;
}

 inline static String POSIXErrorInfo(int errno){
  switch (errno) {
    case ENOENT:
      return "No such file or directory";
    case EEXIST:
      return "File or directory already exists";
    case EIO:
      return "Input/output error";
    case ENOTDIR:
      return "Not a directory";
    case EISDIR:
      return "Is a directory";
    case ENFILE:
      return "Too many open files in system";
    case EMFILE:
      return "Too many open files";
    case ENOSPC:
      return "No space left on device";
    case ENAMETOOLONG:
      return "File name too long";
    case EPERM:
      return "Operation not permitted";
    case ENODEV:
      return "No such device";
    case ENOTBLK:
      return "Not a block device";
    case EBUSY:
      return "Device or resource busy";
    case EAGAIN:
      return "Resource temporarily unavailable";
    case ENXIO:
      return "No such device or address";
    case ENOMEM:
      return "Out of memory";
    case EACCES:
      return "Permission denied";
    case EROFS:
      return "Read-only file system";
    case EINVAL:
      return "Invalid argument";
    default:
      return "Undefined Error";
    }
}

enum FileMode { READ, WRITE, APPEND };

#endif 