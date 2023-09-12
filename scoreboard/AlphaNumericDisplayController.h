#ifndef ALPHANUMERICDISPLAYCONTROLLER_H
#define ALPHANUMERICDISPLAYCONTROLLER_H

// Check if Arduino.h is already included
#ifndef ARDUINO_H
  #include <Arduino.h>
#endif // ARDUINO_H

#ifndef STRING_INCLUDED
#define STRING_INCLUDED
#include <string>
#endif // STRING_INCLUDED

#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED
#include <vector>
#endif // VECTOR_INCLUDED

#ifndef IOSTREAM_INCLUDED
#define IOSTREAM_INCLUDED
#include <iostream>
#endif // IOSTREAM_INCLUDED

#include <algorithm>


class AlphaNumericDisplayController {
public:
    AlphaNumericDisplayController();
    bool begin();
    void writeDataToDisplay(const std::vector<char> dataToDisplay, std::string arduinoSetupName);
    void displayTest(std::string arduinoSetupName);
    void printArduinoSetup();
    void addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin);
    
private:
    byte getDigitByte(const char input);
    std::string formatLength(std::string inputString, int formatLength);
    int getPinByNames(const std::string& setupName, const std::string& pinName);
    std::vector<char> formatAndConcatenateDisplayData(const std::vector<std::pair<std::string, int>> data);
};

#endif
