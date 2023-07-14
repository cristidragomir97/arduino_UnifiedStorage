#include "src/UnifiedStorage.h"
#include <vector>

#define INTERNAL_WRITE_LED LED_D1
#define USB_MOUNTED_LED LED_D0

Doc logFile;
Doc backupFile;
Doc lastUpdateFile;

InternalStorage internalStorage = InternalStorage();
USBStorage usbStorage = USBStorage();

unsigned long bytesWritten = 0;
unsigned long lastLog = 0;
unsigned long lastBackup = 0;
unsigned long lastDisplay = 0;

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
  lastUpdateFile.changeMode(FileMode::READ);  // Open the last update file in read mode
  int lastUpdateBytes = lastUpdateFile.readAsString().toInt();  // Read the last update size from the file
  Serial.print("Reading last update size: ");
  Serial.println(lastUpdateBytes);

  Serial.print("Available update size: ");
  Serial.println(bytesWritten);

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

  while (logFile.available()) {
    int data = logFile.read();  // Read a byte from the log file

    if (data != -1) {
      backupFile.write(data);  // Write the byte to the backup file
      bytesMoved++;
    }
  }

  Serial.print(bytesMoved);
  Serial.println(" bytes moved to USB");
  backupFile.close();  // Close the backup file

  lastUpdateFile.changeMode(FileMode::WRITE);  // Open the last update file in write mode
  lastUpdateFile.write(String(bytesWritten));  // Write the updated last update size to the file
  backupFile.close();
  lastUpdateFile.close();
  usbStorage.unmount();  // Unmount the USB storage
  digitalWrite(USB_MOUNTED_LED, LOW);
}

// Function to backup data to USB storage
void backupToUSB() {
  if (usbStorage.isAvailable()) {
    Serial.println("USB STORAGE IS AVAILABLE");
    if (!usbStorage.isConnected()) {
      usbStorage.begin();  // Initialize the USB storage
      digitalWrite(USB_MOUNTED_LED, HIGH);
      Serial.println("USB STORAGE ENABLED");
      Directory usbRoot = usbStorage.getRootFolder();  // Get the root folder of the USB storage

      backupFile = usbRoot.createFile("backup_file.txt", FileMode::APPEND);  // Create or open the backup file
      lastUpdateFile = usbRoot.createFile("diff.txt", FileMode::READ);  // Create or open the last update file

      performUpdate();
    } else if (usbStorage.isConnected()) {
      performUpdate();
    }
  } else {
    Serial.println("USB Mass storage Not available");
  }
}

// Function to show file systems
void showFileSystems() {
  printTree(internalStorage.getRootFolder());
  if (usbStorage.isConnected()) {
    printTree(usbStorage.getRootFolder());
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(INTERNAL_WRITE_LED, OUTPUT);
  pinMode(USB_MOUNTED_LED, OUTPUT);

  if (!internalStorage.begin() == 0) {
    Serial.println("Failed to initialize internal storage");
    return;
  } else {
    Serial.println("Initialized storage");
  }

  logFile = internalStorage.getRootFolder().createFile("log.txt", FileMode::APPEND);  // Create or open the log file
}

void loop() {
  usbStorage.checkConnection();
  runPeriodically(logData, 100, &lastLog);
  runPeriodically(backupToUSB, 10000, &lastBackup);
}
