#ifndef Directory_H
#define Directory_H

#include "Types.h"
#include "Doc.h"

class Doc;

/**
 * @class Directory
 * @brief Class representing a directory.
 */
class Directory {

  public:
    /*
     * @brief Blank Constructor.
     */
    Directory();

    /*
     * @brief Constructor.
     * @param const char * dirname - The name of the directory.
     */
    Directory(const char * dirname);

    /*
     * @brief Constructor.
     * @param String dirname - The name of the directory.
     */
    Directory(String dirname);

    /*
     * @brief Creates a file inside the directory.
     * @param const char * fileName - The name of the file to create.
     * @return A File object if successfull, NULL if not. 
     */
    Doc createFile(const char * fileName, FileMode fmode);

   /*
     * @brief Creates a file inside the directory.
     * @param String fileName - The name of the file to create.
     * @return A File object if successfull, NULL if not. 
     */
    Doc createFile(String fileName, FileMode fmode);

    /*
     * @brief Removes a directory.
     * @param dirname The name of the directory to remove.
     * @return True if the directory was removed successfully, false otherwise.
     */
    bool remove();

    /*
     * @brief Renames a directory.
     * @param const char * newDirname The new name of the directory.
     * @return True if the directory was renamed successfully, false otherwise.
     */
    bool rename(const char * newDirname);

    /*
     * @brief Renames a directory.
     * @param String newDirname The new name of the directory.
     * @return True if the directory was renamed successfully, false otherwise.
     */
    bool rename(String newDirname);

    /*
     * @brief Checks if the directory exists.
     * @return True if the directory exists, false otherwise.
     */
    bool exists();

    /*
     * @brief Returns the path of the file.
     * @return The path of the file as a const char * 
     */
    const char * getPath();

    /*
     * @brief Returns the path of the file.
     * @return The path of the file as an Arduino String
    */
    String getPathString();

    /*
     * @brief Creates a subfolder in the directory.
     * @param const char * subfolderName - he name of the subfolder to create.
     * @return The created subfolder.
    */
    Directory createSubfolder(const char * subfolderName);

    /*
     * @brief Creates a subfolder in the directory.
     * @param String subfolderName - he name of the subfolder to create.
     * @return The created subfolder.
     */
    Directory createSubfolder(String subfolderName);

    /*
     * @brief Returns File objects for all files in the current dirctory.
     * @return A std::vector of File objects representing the files in the directory.
     */
    std::vector<Doc> getFiles();

   /*
     * @brief Returns Directory objects for all files in the current dirctory.
     * @return A std::vector of Directory objects representing the files in the directory.
     */
    std::vector<Directory>  getFolders();

    /*
     * @brief Copies the current directory
     * @param Directory destination - a Directory object representing the destination
     * @return True upon success, false otherwise. 
     */
    bool copyTo(Directory destination);

    /*
     * @brief Copies the current directory
     * @param const char * destination - the path of the destination location
     * @return True upon success, false otherwise. 
     */
    bool copyTo(const char * destination);

    /*
     * @brief Copies the current directory
     * @param String destination - the path of the destination location
     * @return True upon success, false otherwise. 
     */
    bool copyTo(String destination);

    /*
     * @brief Moves the current directory
     * @param Directory destination - a Directory object representing the destination
     * @return True upon success, false otherwise. 
     */
    bool moveTo(Directory destination);

    /*
     * @brief Moves the current directory
     * @param const char * destination - the path of the destination location
     * @return True upon success, false otherwise. 
    */
    bool moveTo(const char * destination);

    /*
     * @brief Move the current directory
     * @param String destination - the path of the destination location
     * @return True upon success, false otherwise. 
     */
    bool moveTo(String destination);

  private: 
  
    char * dirname;
    char * path;
    DIR * dir;
};

#endif