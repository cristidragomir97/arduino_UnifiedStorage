
#include "Arduino.h"

#ifndef Directory_H
#define Directory_H




/**
 * @class Directory
 * @brief Class representing a directory.
 */
class Directory {
  public:
    Directory();
    /**
     * @brief Constructor.
     * @param dirname The name of the directory.
     */
    Directory(const char * dirname);

    /**
     * @brief Creates a directory.
     * @param dirname The name of the directory to create.
     * @return True if the directory was created successfully, false otherwise.
     */
    bool create(const char * dirname);

    /**
     * @brief Removes a directory.
     * @param dirname The name of the directory to remove.
     * @return True if the directory was removed successfully, false otherwise.
     */
    bool remove(const char * dirname);

    /**
     * @brief Renames a directory.
     * @param newDirname The new name of the directory.
     * @return True if the directory was renamed successfully, false otherwise.
     */
    bool rename(const char * newDirname);

    /**
     * @brief Checks if the directory exists.
     * @return True if the directory exists, false otherwise.
     */
    bool exists();

    /**
     * @brief Returns the path of the file.
     * @return The path of the file.
     */
    const char * getPath();

    // TODO: Also support Arduino String instead along with const char * 

    /**
     * @brief Creates a subfolder in the directory.
     * @param subfolderName The name of the subfolder to create.
     * @return The created subfolder.
     */
    Directory createSubfolder(const char * subfolderName);

    /**
     * @brief Removes a subfolder from the directory.
     * @param subfolderName The name of the subfolder to remove.
     * @return True if the subfolder was removed successfully, false otherwise.
     */
    bool removeSubfolder(const char * subfolderName);

    /**
     * @brief Renames a subfolder in the directory.
     * @param oldName The current name of the subfolder.
     * @param newName The new name of the subfolder.
     * @return True if the subfolder was renamed successfully, false otherwise.
     */
    bool renameSubfolder(const char * oldName,
      const char * newName);

    /**
     * @brief Checks if the subfolder exists in the directory.
     * @param subfolderName The name of the subfolder.
     * @return True if the subfolder exists, false otherwise.
     */
    bool existsSubfolder(const char * subfolderName);

    /**
     * @brief Lists all files in the directory.
     */
    void listFiles();

    /**
     * @brief Lists all subfolders in the directory.
     */
    void listSubfolders();

  private: 
    char * dirname;
    char * path;
    DIR * dir;
};

#endif