# EEPROMIndex Library

This library simplifies the management of data stored in the EEPROM of Arduino boards. It provides an easy-to-use interface for adding, writing, reading, and clearing data, along with features for index management and data validation. This library is one of core in ESP Template System (later)

## WARNING!:
This library for my personal usage. NOT FOR PRODUCTION USE. May not stable until this message removed as I need add or remove functions based on own requirement.

## Dependency
Need `<Arduino.h>` and `<EEPROM.h>`. This libraries is preinstall in Arduino IDE (if I not mistaken)

## Features:
* Manages indexes for organized data storage
* Validates string inputs for printable ASCII characters
* Offers optional debug output for troubleshooting
* Supports clearing the entire EEPROM

## Installation:
* Download the library zip file from this repository.
* Unzip the file and copy the EEPROMIndexfolder into your Arduino libraries directory.
* Restart the Arduino IDE.

## Usage:

Include the library:
```C
#include "EEPROMMIndex.h"
```

Create an instance:
```C
EEPROMIndex index(512, 65, 0, true); // 512 bytes of EEPROM, 65 max value size, start address ,debug enabled
```

Define indexes:
```C
index.addIndex("WifiSSID", 32) // Create index name "WifiSSID" with size of 32
index.addIndex("WifiPass", 64); // Create index name "WifiPass" with size of 64
```

Write values:
```C
index.writeMemory("WifiSSID", "Testing");
index.writeMemory("WifiPass", "P@ssword");
```

Read values:
```C
  char ssid[33];
  char pass[33];

  index.readMemory("WifiSSID", ssid, sizeof(ssid));
  index.readMemory("WifiPass", pass, sizeof(pass));

  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(pass);
```

Clear EEPROM:
```C
index.clearEEPROM();
```
