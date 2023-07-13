class Doc{
public:

  */
  /**
   * @brief Constructor.
   * @param const char * path - path of the file
  */
   /**
   * @brief Closes the file, and opens it again with a new file mode
   * @param mode The file mode (READ, WRITE, or APPEND).
   * @return True if operation was successful, false otherwise.
   */
   * @param const char * filename - The name of the file to open.
  */
  /**
   * @brief Opens a file with the specified mode.
   * @param String filename - The name of the file to open.
   * @param mode The file mode (READ, WRITE, or APPEND).
   * @return True if the file was opened successfully, false otherwise.
  */
   * @brief Reads the contents of the file as an Arduino String.
   * @return The file contents as a String.
   * @param const char * newDocname The new name of the file.
  /**
   * @brief Renames the file.
   * @param String newDocname The new name of the file.
   * @return True if the file was renamed successfully, false otherwise.
   */
  * @param const char * destinationPath - The destination path to copy the file to.
  * @return True upon success, false otherwise. 
  /**
  * @brief Copies the file to the specified destination path.
  * @param String destinationPath The destination path to copy the file to.
  * @return True upon success, false otherwise. 
  */
  /**
  * @brief Copies the file to the specified destination path.
  * @param Directory destinationPath - The destination folder to copy the file to.
  * @return True upon success, false otherwise. 
  * @param const char * destinationPath The destination path to move the file to.
  * @return True upon success, false otherwise. 
  /**
  * @brief Moves the file to the specified destination path.
  * @param String destinationPath The destination path to move the file to.
  * @return True upon success, false otherwise. 
  */
  * @param Directory destinationFolder The destination directory to move the file to.
  * @return True upon success, false otherwise. 
  /*
     * @brief Returns a reference to the parent folder of the current folder. 
     * @return A directory object representing the current folder. 
  */
  /*
     * @brief Returns the path of the directory.
     * @return The path of the file as a const char * 
  */
  /*
     * @brief Returns the path of the directory.
     * @return The path of the file as a String
  */
  size_t write(uint8_t value);