



#ifndef Directory_H
#define Directory_H

#include "Types.h"
#include "Doc.h"

class Doc;


namespace UniStdWrapper {
  #include <unistd.h>
}




/**
 * @class Directory
 * @brief Class representing a directory.
 */
class Directory {

  public:
    /**
     * @brief Blank Constructor.
     */
    Directory();
    /**
     * @brief Constructor.
     * @param const char * dirname The name of the directory.
     */
    Directory(const char * dirname);


      /**
     * @brief Constructor.
     * @param string dirname The name of the directory.
     */
    Directory(String dirname);

    /**
     * @brief Creates a directory.
     * @param dirname The name of the directory to create.
     * @return True if the directory was created successfully, false otherwise.
     */

    Doc createFile(const char * fileName, FileMode fmode);

    Doc createFile(String fileName, FileMode fmode);

    /**
     * @brief Removes a directory.
     * @param dirname The name of the directory to remove.
     * @return True if the directory was removed successfully, false otherwise.
     */
    bool remove();

    /**
     * @brief Renames a directory.
     * @param newDirname The new name of the directory.
     * @return True if the directory was renamed successfully, false otherwise.
     */
    bool rename(const char * newDirname);

    bool rename(String newDirname);

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

    String getPathString();

    // TODO: Also support Arduino String instead along with const char * 

    /**
     * @brief Creates a subfolder in the directory.
     * @param subfolderName The name of the subfolder to create.
     * @return The created subfolder.
     */
    Directory createSubfolder(const char * subfolderName);


    Directory createSubfolder(String subfolderName);

    /**
     * @brief Removes a subfolder from the directory.
     * @param subfolderName The name of the subfolder to remove.
     * @return True if the subfolder was removed successfully, false otherwise.
     */
    std::vector<Doc> getFiles();

    /**
     * @brief Lists all subfolders in the directory.
     */
    std::vector<Directory>  getFolders();

    bool copyTo(Directory destination);

    bool copyTo(const char * destination);

    bool copyTo(String destination);

    bool moveTo(Directory destination);

    bool moveTo(const char * destination);

    bool moveTo(String destination);

  private: 
  
    char * dirname;
    char * path;
    DIR * dir;
};

#endif