/*
This example demonstrates the usage of the "UnifiedStorage" library for logging and backing up data to USB storage in case a USB Mass Storage device is inserted. 

The code defines two main functions: "logData" and "performUpdate". 
The "logData" function logs sensor data by reading an analog sensor and writing the data to the log file.

The "performUpdate" function performs the update process by:
*  reading the last update size from a file (number of bytes)
*  copying the new data from the log file to a backup file 
*  and updating the last update size.

INSTRUCTIONS
* Make sure the QSPI storage of your board is properly partitioned. 
    * You can do that by flashing the QSPIFormat example that can be found in the STM32H747_System folder
    * Open the serial monitor and select answer with "Y" when this appears "Do you want to use partition scheme 1? Y/[n]"
    * Reboot the board
* Connect a RS485-enabled device to see the debugging output. 
* This sketch will log data, and check if there is any USB MSD Device connected to the USB Port of the Opta. 
  The USB device is mounted and unmounted after every update operation. The first status LED is on when the USB drive is mounted. 
  So as long as the status LED is off you can safely remove the drive. 
  The skecth will log to internal storage in the meantime, and wait for the USB drive to be inserted again. 
*/

#include <ArduinoRS485.h>
#include "UnifiedStorage.h"
#include <vector>

#define INTERNAL_WRITE_LED LED_D1
#define USB_MOUNTED_LED LED_D0

constexpr auto baudrate { 115200 };


Doc logFile;
Doc backupFile;
Doc lastUpdateFile;

InternalStorage internalStorage = InternalStorage();
USBStorage usbStorage = USBStorage();

unsigned long bytesWritten = 0;
unsigned long lastLog = 0;
unsigned long lastBackup = 0;
unsigned long lastDisplay = 0;

size_t printRS485(char* message)
{
    RS485.beginTransmission();
    auto len = strlen(message);
    RS485.write(message, len);
    RS485.endTransmission();
}

size_t printlnRS485(char* message)
{
    printRS485(message);
    RS485.beginTransmission();
    RS485.write('\r');
    RS485.write('\n');
    RS485.endTransmission();
}

void configureRS485(const int baudrate){
    const auto bitduration { 1.f / baudrate };
    const auto wordlen { 9.6f }; // OR 10.0f depending on the channel configuration
    const auto preDelayBR { bitduration * wordlen * 3.5f * 1e6 };
    const auto postDelayBR { bitduration * wordlen * 3.5f * 1e6 };

    RS485.begin(baudrate);
    RS485.setDelays(preDelayBR, postDelayBR);
    RS485.noReceive();
}

// Function to run a given method periodically
void runPeriodically(void (*method)(), unsigned long interval, unsigned long* variable) {
  unsigned long currentMillis = millis();

  if (currentMillis - *variable >= interval) {
    *variable = currentMillis;
    method();  // Call the provided method
  }
}

// Function to log sensor data
void logData() {
  digitalWrite(INTERNAL_WRITE_LED, HIGH);
  int timeStamp = millis();
  int sensorReading = analogRead(A0);
  String line = String(timeStamp) + "," + String(sensorReading) + "\n";
  bytesWritten += logFile.write(line);  // Write the log line to the file
  digitalWrite(INTERNAL_WRITE_LED, LOW);
}

// Function to perform the update process
void performUpdate() {
  printlnRS485("Opening diff file");
  lastUpdateFile.changeMode(FileMode::READ);  // Open the last update file in read mode
  int lastUpdateBytes = lastUpdateFile.readAsString().toInt();  // Read the last update size from the file

  if (lastUpdateBytes > bytesWritten) {
    logFile.changeMode(FileMode::WRITE);  // Open the log file in write mode
    logFile.write(String(0));  // Reset the log file by writing 0 as the last update size
    lastUpdateBytes = 0;
  }

  logFile.changeMode(FileMode::READ);  // Open the log file in read mode
  logFile.seek(lastUpdateBytes);  // Move the file pointer to the last update position
  backupFile.changeMode(FileMode::APPEND);  // Open the backup file in append mode
  unsigned long bytesMoved = 0;
  unsigned long totalBytesToMove = bytesWritten - lastUpdateBytes;
  unsigned int progressPercentage = 0;
  printlnRS485("Ready to copy data");

  while (logFile.available()) {
    int data = logFile.read();  // Read a byte from the log file

    if (data != -1) {
      backupFile.write(data);  // Write the byte to the backup file
      bytesMoved++;
    }
  }
  printlnRS485("Succesfully copied data");

  backupFile.close();  // Close the backup file

  lastUpdateFile.changeMode(FileMode::WRITE);  // Open the last update file in write mode
  lastUpdateFile.write(String(bytesWritten));  // Write the updated last update size to the file
  backupFile.close();
  lastUpdateFile.close();

   printlnRS485("Succesfully updated diff file");
  usbStorage.unmount();  // Unmount the USB storage

  digitalWrite(USB_MOUNTED_LED, LOW);
}

// Function to backup data to USB storage
void backupToUSB() {
  if (usbStorage.isAvailable()) {
    printlnRS485("USB Mass storage is available");
    if (!usbStorage.isConnected()) {
      digitalWrite(USB_MOUNTED_LED, HIGH);
      printlnRS485("Mounting USB Mass Storage");
      usbStorage.begin();  // Initialize the USB storage
      Directory usbRoot = usbStorage.getRootFolder();  // Get the root folder of the USB storage

      backupFile = usbRoot.createFile("backup_file.txt", FileMode::APPEND);  // Create or open the backup file
      lastUpdateFile = usbRoot.createFile("diff.txt", FileMode::READ);  // Create or open the last update file

      performUpdate();
    } else if (usbStorage.isConnected()) {
      printlnRS485("USB Mass storage is connected, performing update");
      performUpdate();

    }
  }
}


void setup() {
  //Serial.begin(115200);

  pinMode(INTERNAL_WRITE_LED, OUTPUT);
  pinMode(USB_MOUNTED_LED, OUTPUT);

  configureRS485(baudrate);
  printlnRS485("RS485 goes brrr...");

  if (!internalStorage.begin() == 0) {
    printlnRS485("Failed to initialize internal storage");
    return;
  } else {
    printlnRS485("Initialized storage");
  }

  logFile = internalStorage.getRootFolder().createFile("log.txt", FileMode::APPEND);  // Create or open the log file
  printlnRS485("Created Log file");
}

void loop() {
  usbStorage.checkConnection();
  runPeriodically(logData, 100, &lastLog);
  runPeriodically(backupToUSB, 10000, &lastBackup);
}
