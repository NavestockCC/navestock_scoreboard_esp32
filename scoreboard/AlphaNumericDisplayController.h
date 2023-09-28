#ifndef ALPHANUMERICDISPLAYCONTROLLER_H
#define ALPHANUMERICDISPLAYCONTROLLER_H

#pragma once

#include <Arduino.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>


class AlphaNumericDisplayController {
public:
    AlphaNumericDisplayController();
    bool begin();
    void addShiftRegisterPinSetup(byte pinArray[7]);
    void addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin);
    void printArduinoSetup();
    void writeDataToDisplay(const std::vector<std::pair<std::string, int>> dataToDisplay, std::string arduinoSetupName);
    void displayTest(std::string arduinoSetupName, int displayLength);
    void rgbBlink(int numberOfBlinks, int blinkSpeed, std::vector<byte> colours);
    void clearDisplay(std::string arduinoSetupName, int displayLength);
    
private:
    byte getDigitByte(const char input);
    byte* getShiftRegisterPins();
    std::string formatLength(std::string inputString, int formatLength);
    int getPinByNames(const std::string& setupName, const std::string& pinName);
};

#endif
