# EEPROMIndex Library

This library simplifies the management of data stored in the EEPROM of Arduino boards. It provides an easy-to-use interface for adding, writing, reading, and clearing data, along with features for index management and data validation.

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
`#include "EEPROMMIndex.h"`

Create an instance:
`EEPROMIndex index(512, 64, true); // 512 bytes of EEPROM, 64 max value size, debug enabled`

Define indexes:
`index.addIndex("WifiSSID", 0, 32);`
`index.addIndex("WifiPass", 32, 32);`

Write values:
`index.writeMemory("WifiSSID", "Testing");`
`index.writeMemory("WifiPass", "P@ssword");`

Read values:
```
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
`index.clearEEPROM();`