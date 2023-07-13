#ifndef Doc_H
#define Doc_H


#include "Types.h"
#include <Arduino.h>
#include "Directory.h"
#include <string.h>


class Directory;



class Doc{
public:

  /**
   * @brief Constructor.
  */
  Doc();

  /**
   * @brief Constructor.
   * @param const char * path - path of the file
  */
  Doc(const char* path);

   /**
   * @brief Closes the file, and opens it again with a new file mode
   * @param mode The file mode (READ, WRITE, or APPEND).
   * @return True if operation was successful, false otherwise.
   */
  bool changeMode(FileMode mode);

  /**
   * @brief Opens a file with the specified mode.
   * @param const char * filename - The name of the file to open.
   * @param mode The file mode (READ, WRITE, or APPEND).
   * @return True if the file was opened successfully, false otherwise.
  */
  bool open(const char* filename, FileMode mode);

  /**
   * @brief Opens a file with the specified mode.
   * @param String filename - The name of the file to open.
   * @param mode The file mode (READ, WRITE, or APPEND).
   * @return True if the file was opened successfully, false otherwise.
  */
  bool open(String filename, FileMode mode);


  /**
   * @brief Closes the file and releases any allocated resources.
   */
  void close();

  /**
   * @brief Seeks to a specific position in the file.
   * @param offset The offset from the beginning of the file.
   * @return True if the seek operation was successful, false otherwise.
   */
  bool seek(size_t offset);

  /**
   * @brief Reads data from the file into a buffer.
   * @param buffer The buffer to read data into.
   * @param size The size of the buffer.
   * @return The number of bytes read from the file.
   */
  size_t read(uint8_t* buffer, size_t size);

  /**
   * @brief Reads the contents of the file as an Arduino String.
   * @return The file contents as a String.
   */
  String readAsString();

  /**
   * @brief Writes data to the file from a buffer.
   * @param buffer The buffer containing the data to write.
   * @param size The size of the data to write.
   * @return The number of bytes written to the file.
   */
  size_t write(const uint8_t* buffer, size_t size);

  /**
   * @brief Writes a string to the file.
   * @param data The string to write.
   * @return The number of bytes written to the file.
   */
  size_t write(String);

  /**
   * @brief Removes the file.
   * @return True if the file was removed successfully, false otherwise.
   */
  bool remove();

  /**
   * @brief Renames the file.
   * @param const char * newDocname The new name of the file.
   * @return True if the file was renamed successfully, false otherwise.
   */
  bool rename(const char* newDocname);

  /**
   * @brief Renames the file.
   * @param String newDocname The new name of the file.
   * @return True if the file was renamed successfully, false otherwise.
   */
  bool rename(String newDocname);

  /**
   * @brief Checks if the file exists.
   * @return True if the file exists, false otherwise.
   */
  bool exists();

  /**
  * @brief Copies the file to the specified destination path.
  * @param const char * destinationPath - The destination path to copy the file to.
  * @return True upon success, false otherwise. 
  */
  bool copyTo(const char* destinationPath);

  /**
  * @brief Copies the file to the specified destination path.
  * @param String destinationPath The destination path to copy the file to.
  * @return True upon success, false otherwise. 
  */
  bool copyTo(String destinationPath);

  /**
  * @brief Copies the file to the specified destination path.
  * @param Directory destinationPath - The destination folder to copy the file to.
  * @return True upon success, false otherwise. 
  */
  bool copyTo(Directory * destinationFolder);

  /**
  * @brief Moves the file to the specified destination path.
  * @param const char * destinationPath The destination path to move the file to.
  * @return True upon success, false otherwise. 
  */
  bool moveTo(const char * destinationPath);

  /**
  * @brief Moves the file to the specified destination path.
  * @param String destinationPath The destination path to move the file to.
  * @return True upon success, false otherwise. 
  */
  bool moveTo(String destinationPath);

  /**
  * @brief Copies the file to the specified destination folder.
  * @param Directory destinationFolder The destination directory to move the file to.
  * @return True upon success, false otherwise. 
  */
  bool moveTo(Directory * destinationFolder);

  /*
     * @brief Returns a reference to the parent folder of the current folder. 
     * @return A directory object representing the current folder. 
  */
  Directory getParentFolder();

  /*
     * @brief Returns the path of the directory.
     * @return The path of the file as a const char * 
  */
  const char * getPath();

  /*
     * @brief Returns the path of the directory.
     * @return The path of the file as a String
  */
  String getPathString();

  // Inherited methods from Stream
  int available() ;
  int read() ;
  size_t write(uint8_t value);


private:
  FILE* fp;
  uint8_t* readBuffer;
  char* path;
  FileMode fm;
};

#endif