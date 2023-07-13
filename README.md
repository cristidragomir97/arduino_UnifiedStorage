# arduino_UnifiedStorage


The UnifiedStorage library provides a unified interface to access different types of storage, including internal storage, SD cards, and USB mass storage devices. 
It simplifies the handling of files and directories across multiple storage mediums.

## Features

- Access files and directories on QSPI Flash, SD cards, and USB mass storage devices.
- Read and write data to files, as raw bytes, chars or Strings. 
- Create, remove, and rename files and directories.
- Copy and move files and directories.
- List files and subfolders in a directory.
- Manipulate folders from one storage medium to another. 



## Compatibility 
This library is compatible with STM32 and Renesas based Arduino boards. However the storage mediums available depend on the hardware interfaces.
* Portenta Machine Control - USB and Internal QSPI Flash
* Portenta H7 + Portenta Breakout - USB, SD and QSPI
* Portenta H7 + Vision Shield - SD and QSPI
* Arduino Giga R1 - USB and QSPI
* Arduino Opta - USB and QSPI
* Portenta C33 + Portenta Breakout - USB, SD and QSPI
* Portenta C33 + Vision Shield - SD and QSPI

