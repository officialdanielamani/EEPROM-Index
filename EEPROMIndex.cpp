#include "EEPROMIndex.h"

EEPROMIndex::EEPROMIndex(int eepromSize, int maxValueSize, int startingAddress, bool debug)
: eepromSize(eepromSize), maxValueSize(maxValueSize), numOfIndexes(0), maxSizeOfIndexes(10), debug(debug), nextStartAddress(startingAddress) {
  indexes = new Index[maxSizeOfIndexes];
  EEPROM.begin(eepromSize);
}

EEPROMIndex::~EEPROMIndex() {
  delete[] indexes;
}

void EEPROMIndex::begin() {
  // Optional initialization procedures here
  // (Currently empty)
}

void EEPROMIndex::addIndex(const char* indexName, int size) {
  if (nextStartAddress + size > eepromSize) {
    if (debug) Serial.println("Error: EEPROM is full.");
    return;
  }

  indexes[numOfIndexes++] = {String(indexName), nextStartAddress, size};
  nextStartAddress += size;

  if (debug) {
    Serial.print("\nIndex added: ");
    Serial.print(indexName);
    Serial.print(", Addr: ");
    Serial.print(indexes[numOfIndexes - 1].startAddress);
    Serial.print(", Size: ");
    Serial.println(indexes[numOfIndexes - 1].size);
  }
}

void EEPROMIndex::writeMemory(const char* indexName, const char* value) {
  EEPROM.begin(eepromSize);//Needed as ESP8266 not function write EEPROM if in function?
  if (!validateString(value)) {
    if (debug) Serial.println("Error: String contains non-printable ASCII characters.");
    return;
  }

  EEPROMIndex::Index* index = findIndexByName(indexName);
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

  if (debug) {
    Serial.print("\nWrote to: ");
    Serial.print(indexName);
    Serial.print(" , Val: ");
    Serial.println(value);
  }

  EEPROM.commit();
}

void EEPROMIndex::readMemory(const char* indexName, char* output, int maxLen) {
  EEPROM.begin(eepromSize);
  EEPROMIndex::Index* index = findIndexByName(indexName);
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

void EEPROMIndex::clearEEPROM() {
  EEPROM.begin(eepromSize);
  for (int i = 0; i < eepromSize; i++) {
    EEPROM.write(i, 0); // Clearing the EEPROM
  }
  EEPROM.commit();
  if (debug) Serial.println("Done clearing EEPROM");
}

bool EEPROMIndex::isPrintableASCII(char c) {
  return c >= 32 && c <= 126;
}

bool EEPROMIndex::validateString(const char* value) {
  while (*value) {
    if (!isPrintableASCII(*value++)) return false;
  }
  return true;
}

EEPROMIndex::Index* EEPROMIndex::findIndexByName(const char* indexName) {
    for (int i = 0; i < numOfIndexes; i++) {
        if (indexes[i].name.equals(indexName)) { // Use .equals for String comparison in Arduino
            return &indexes[i];
        }
    }
    return nullptr; // Return null if the index is not found
}

void EEPROMIndex::resizeIndexesArray() {
    int newSize = maxSizeOfIndexes * 2; // Double the size
    Index* newIndexArray = new Index[newSize];
    for (int i = 0; i < numOfIndexes; i++) {
        newIndexArray[i] = indexes[i]; // Copy existing indexes
    }
    delete[] indexes; // Free old array
    indexes = newIndexArray;
    maxSizeOfIndexes = newSize;
}