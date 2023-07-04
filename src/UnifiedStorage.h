#ifndef UNIFIED_STORAGE_H
#define UNIFIED_STORAGE_H

#include "Arduino.h"
#include "USBStorage.h"
#include "SDStorage.h"
#include "InternalStorage.h"
#include "File.h"
#include "Directory.h"

USBStorage usbStorage = USBStorage();
SDStorage sdStorage = SDStorage();
InternalStorage internalStorage = InternalStorage();