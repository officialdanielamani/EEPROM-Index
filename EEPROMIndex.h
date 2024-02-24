#ifndef EEPROMIndex_h
#define EEPROMIndex_h

#include <Arduino.h>
#include <EEPROM.h>

class EEPROMIndex {
public:
  // Constructor with EEPROM size, max value size, optional starting address and debug mode
  EEPROMIndex(int eepromSize, int maxValueSize, int startingAddress = 0, bool debug = false);
  // Destructor to clean up dynamically allocated memory
  ~EEPROMIndex();
  // Placeholder for potential initialization procedures
  void begin();
  // Add a new index
  void addIndex(const char* indexName, int size);
  // Write a value to a specific index
  void writeMemory(const char* indexName, const char* value);
  // Read a value from a specific index
  void readMemory(const char* indexName, char* output, int maxLen);
  // Method to clear the EEPROM
  void clearEEPROM();
  // Validate that a string is printable ASCII
  bool validateString(const char* value);
  // Total EEPROM size
  int eepromSize;
  // Maximum size of values to be written/read
  int maxValueSize;

private:
  // Structure to hold index information
  struct Index {
    String name;
    int startAddress;
    int size;
  };
  // Dynamically allocated array of indexes
  Index* indexes;
  // Number of indexes currently in use
  int numOfIndexes;
  // Capacity of the indexes array
  int maxSizeOfIndexes;
  // Debug mode flag
  bool debug;
  // Next available starting address for new indexes
  int nextStartAddress;
  // Utility to find an index by name
  Index* findIndexByName(const char* indexName);
  // Resize the indexes array if needed
  void resizeIndexesArray();
  // Check if character is printable ASCII
  bool isPrintableASCII(char c);
};

#endif /* EEPROMIndex_h */
