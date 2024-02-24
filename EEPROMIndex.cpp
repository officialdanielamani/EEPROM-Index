#include "EEPROMManagement.h"

EEPROMManagement::EEPROMManagement(int eepromSize, int maxValueSize, bool debug)
: eepromSize(eepromSize), maxValueSize(maxValueSize), numOfIndexes(0), maxSizeOfIndexes(10), debug(debug) {
    indexes = new Index[maxSizeOfIndexes];
    EEPROM.begin(eepromSize);
}

EEPROMManagement::~EEPROMManagement() {
    delete[] indexes; // Clean up the dynamically allocated memory
}

void EEPROMManagement::addIndex(const char* indexName, int startAddress, int size) {
    if (numOfIndexes >= maxSizeOfIndexes) {
        resizeIndexesArray(); // Resize the array if there's not enough space
    }
    // Add the new index
    indexes[numOfIndexes++] = {String(indexName), startAddress, size};
    if (debug){
      Serial.print( "\nIndex: ");
      Serial.print(indexName);
      Serial.print(" , Addr: ");
      Serial.print(startAddress);
      Serial.print(" , Size: ");
      Serial.println(size);
    }
}

void EEPROMManagement::writeMemory(const char* indexName, const char* value) {
  EEPROM.begin(eepromSize);
    if (!validateString(value)) {
        if (debug) Serial.println("Error: String contains non-printable ASCII characters.");
        return;
    }

    EEPROMManagement::Index* index = findIndexByName(indexName);
    if (index == nullptr) {
        if (debug) Serial.println("Error: Index not found.");
        return;
    }

    char existingData[maxValueSize + 1]; // Plus one for null terminator
    readMemory(indexName, existingData, sizeof(existingData));

    if (strcmp(existingData, value) == 0) {
        if (debug) Serial.println("No change detected, skiping");
        return;
    }

    int len = strlen(value);
    len = min(len, index->size - 1); // Ensure there's space for the null terminator
    for (int i = 0; i < len; i++) {
        EEPROM.write(index->startAddress + i, value[i]);
    }
    EEPROM.write(index->startAddress + len, '\0'); // Null terminator
        if (debug){
      Serial.print("\nWrite to: ");
      Serial.print(indexName);
      Serial.print(" , Val: ");
      Serial.println(value);
    }
    EEPROM.commit();
}

void EEPROMManagement::readMemory(const char* indexName, char* output, int maxLen) {
  EEPROM.begin(eepromSize);
    EEPROMManagement::Index* index = findIndexByName(indexName);
    if (index == nullptr) {
        if (debug) Serial.println("Error: Index not found.");
        output[0] = '\0'; // Ensure we return an empty string if the index is not found
        return;
    }

    int len = 0;
    for (int i = 0; i < index->size && len < maxLen - 1; i++) {
        char readChar = EEPROM.read(index->startAddress + i);
        if (readChar == '\0') break;
        output[len++] = readChar;
    }
    output[len] = '\0'; // Ensure null termination
}

void EEPROMManagement::clearEEPROM() {
    for (int i = 0; i < eepromSize; i++) {
        EEPROM.write(i, 0xFF); // Clearing the EEPROM
    }
    EEPROM.commit();
    if (debug) Serial.println("Done clearing EEPROM");
}

bool EEPROMManagement::isPrintableASCII(char c) {
    return c >= 32 && c <= 126;
}

bool EEPROMManagement::validateString(const char* value) {
    while (*value) {
        if (!isPrintableASCII(*value++)) return false;
    }
    return true;
}

EEPROMManagement::Index* EEPROMManagement::findIndexByName(const char* indexName) {
    for (int i = 0; i < numOfIndexes; i++) {
        if (indexes[i].name.equals(indexName)) { // Use .equals for String comparison in Arduino
            return &indexes[i];
        }
    }
    return nullptr; // Return null if the index is not found
}

void EEPROMManagement::resizeIndexesArray() {
    int newSize = maxSizeOfIndexes * 2; // Double the size
    Index* newIndexArray = new Index[newSize];
    for (int i = 0; i < numOfIndexes; i++) {
        newIndexArray[i] = indexes[i]; // Copy existing indexes
    }
    delete[] indexes; // Free old array
    indexes = newIndexArray;
    maxSizeOfIndexes = newSize;
}