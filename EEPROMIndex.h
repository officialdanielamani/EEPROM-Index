#ifndef EEPROMIndex_h
#define EEPROMIndex_h

#include <Arduino.h>
#include <EEPROM.h>

class EEPROMIndex {
public:
    EEPROMIndex(int eepromSize, int maxValueSize = 64, bool debug = false); // Constructor with EEPROM size and optional max value size
    ~EEPROMIndex(); // Destructor to clean up dynamically allocated memory
    void begin(); // Placeholder for potential initialization procedures
    void addIndex(const char* indexName, int startAddress, int size); // Add a new index
    void writeMemory(const char* indexName, const char* value); // Write a value to a specific index
    void readMemory(const char* indexName, char* output, int maxLen); // Read a value from a specific index
    void clearEEPROM(); // Method to clear the EEPROM
    bool validateString(const char* value); // Validate that a string is printable ASCII
    int eepromSize; // Total EEPROM size
    int maxValueSize; // Maximum size of values to be written/read

private:
    struct Index { // Structure to hold index information
        String name;
        int startAddress;
        int size;
    };
    Index* indexes; // Dynamically allocated array of indexes
    int numOfIndexes; // Number of indexes currently in use
    int maxSizeOfIndexes; // Capacity of the indexes array
    bool debug;
    Index* findIndexByName(const char* indexName); // Utility to find an index by name
    void resizeIndexesArray(); // Resize the indexes array if needed
    bool isPrintableASCII(char c); // Check if character is printable ASCII
};

#endif /* EEPROMIndex_h */
