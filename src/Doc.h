#ifndef Doc_H
#define Doc_H


#include "Types.h"
#include <Arduino.h>
#include "Directory.h"
#include <string.h>


class Directory;

class Doc /*: public Stream */{
public:
  /**
   * @enum DocMode
   * @brief Doc modes for opening files.
   */


  /**
   * @brief Constructor.
   */
  Doc();

  Doc(const char* path);


  bool changeMode(FileMode mode);

  /**
   * @brief Opens a file with the specified mode.
   * @param filename The name of the file to open.
   * @param mode The file mode (READ, WRITE, or APPEND).
   * @return True if the file was opened successfully, false otherwise.
   */
  bool open(const char* filename, FileMode mode);


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
   * @brief Reads the contents of the file as a string.
   * @return The file contents as a string.
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
   * @param newDocname The new name of the file.
   * @return True if the file was renamed successfully, false otherwise.
   */
  bool rename(const char* newDocname);

  bool rename(String newDocname);

  /**
   * @brief Checks if the file exists.
   * @return True if the file exists, false otherwise.
   */
  bool exists();

  /**
  * @brief Copies the file to the specified destination path.
  * @param destinationPath The destination path to copy the file to.
  */
  bool copyTo(const char* destinationPath);

  bool copyTo(String destinationPath);


 /**
  * @brief Copies the file to the specified destination folder.
  * @param destinationFolder The destination directory to copy the file to.
  */
  bool copyTo(Directory * destinationFolder);

  /**
  * @brief Moves the file to the specified destination path.
  * @param destinationPath The destination path to move the file to.
  */
  bool moveTo(const char * destinationPath);

  bool moveTo(String destinationPath);

  /**
  * @brief Copies the file to the specified destination folder.
  * @param destinationFolder The destination directory to copy the file to.
  */
  bool moveTo(Directory * destinationFolder);

  //Directory getParentFolder();
  Directory getParentFolder();

  // copy to directory object 
  const char * getPath();

  String getPathString();

  // Inherited methods from Stream
  int available() ;
  int read() ;
  int peek() ;
  size_t write(uint8_t value) ;
//  using Print::write;  // Bring the other write methods from Print into Doc


private:
  FILE* fp;
  uint8_t* readBuffer;
  char* path;
  FileMode fm;
};

#endif