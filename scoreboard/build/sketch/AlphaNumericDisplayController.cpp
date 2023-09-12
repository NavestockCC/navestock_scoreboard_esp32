#line 1 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/AlphaNumericDisplayController.cpp"
#include "AlphaNumericDisplayController.h"

// Define the pins connected to the shift registers

struct PinConfig {
    std::string name;
    int value;
};

std::vector<std::pair<std::string, std::vector<PinConfig>>> arduinoSetup = {};


// Define the pins connected to the 7-segment display (a-g segments)
const byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12};

/* Alpha Numeric display byte definitions

          B00000001
            aaa
B00100000  f   b B00000010
           f   b
B01000000   ggg
           e   c
B00010000  e   c B00000100
            ddd
          B00001000

*/

const byte digits[] = {
    B01110111, // a
    B01111100, // b
    B00111001, // c
    B01011110, // d
    B01111001, // e
    B01110001, // f
    B00111101, // g
    B01110100, // h
    B00000101, // i
    B00011110, // j
    B01110101, // k
    B00111000, // l
    B01010101, // m
    B01010100, // n
    B01011100, // o
    B01110011, // p
    B01100111, // q
    B01010000, // r
    B01101101, // s
    B01111000, // t
    B00011100, // u
    B00111110, // v
    B01101010, // w
    B01110110, // x
    B01101110, // y
    B01011011, // z
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111  // 9
};

AlphaNumericDisplayController::AlphaNumericDisplayController() {
    // Constructor implementation (if needed)
}



 /***************************************************************************************
 * Initialise the Arduino Output Pins
 ****************************************************************************************/
bool AlphaNumericDisplayController::begin(){

    // Validate that arduinoSetup vector is not empty
    if (arduinoSetup.empty()) {
        Serial.println("Error: arduinoSetup vector is empty. Please initialize it before calling begin.");
        return false; // Exit the function if validation fails
    }

    // Validate that segmentPins array is not empty
    if (sizeof(segmentPins) / sizeof(segmentPins[0]) == 0) {
        Serial.println("Error: segmentPins array is empty. Please initialize it before calling begin.");
        return false; // Exit the function if validation fails
    }    

    // Set Arduino pins as outputs
        for (const auto& setup : arduinoSetup) {
            for (const PinConfig& pin : setup.second) {
                pinMode(pin.value, OUTPUT);
        }
    }

    // Set segment pins as outputs
        for (int i = 0; i < 7; i++)
        {
            pinMode(segmentPins[i], OUTPUT);
        }

    return true;
}

byte AlphaNumericDisplayController::getDigitByte(const char input)
{
    // Ensure the input is a valid character
    if ((input >= 'a' && input <= 'z') || (input >= '0' && input <= '9'))
    {
        if (input >= 'a' && input <= 'z')
        {
            // Calculate the index for alphabetic characters ('a' corresponds to 0)
            int index = input - 'a';
            return digits[index];
        }
        else
        {
            // Calculate the index for numerals ('0' corresponds to 26)
            int index = input - '0' + 26;
            return digits[index];
        }
    }
    else
    {
        // Return blank (all segments off) for invalid input
        return B00000000;
    }
}


void AlphaNumericDisplayController::writeDataToDisplay(const std::vector<char> dataToDisplay, std::string arduinoSetupName) {
    int dataPin = getPinByNames(arduinoSetupName, "dataPin");
    int clockPin = getPinByNames(arduinoSetupName, "clockPin");

    // Send a byte of data to 'numShiftRegisters' shift registers in series
    // Reverse the vector
    Serial.print("Data to display: ");
    for (char dispChar : dataToDisplay) {
        Serial.print(dispChar);
            byte digitToDisplay = getDigitByte(dispChar);

            for (int i = 7; i >= 0; i--) { // Iterate over the 8 bits of the character 
            digitalWrite(dataPin, (digitToDisplay >> i) & 1);
            digitalWrite(clockPin, HIGH);
            digitalWrite(clockPin, LOW);
        }

    }
    Serial.println();
}

/*
 * formatAndConcatenateDisplayData(const std::vector<std::pair<int, int>>& data)
 * @param const std::vector<std::pair<std::string, int>> - string of data to display, int display length
 * 
 * @returns std::string of data to the correct length for each item in the vector and concatenated into a single variable.
*/
std::vector<char> AlphaNumericDisplayController::formatAndConcatenateDisplayData(const std::vector<std::pair<std::string, int>> data) {
    std::string result;

    for (const auto pair : data) {
        const std::string str = pair.first;
        int length = pair.second;

        if (length >= 0 && str.length() > 0) {
            result += formatLength(str, length);
        } else {
            // Handle invalid length (out of bounds)
            std::cerr << "Invalid length for string: " << str << std::endl;
            // You can choose to throw an exception or handle this error differently as needed.
        }
    }

    std::vector<char> charVector(result.begin(), result.end());
    std::reverse(charVector.begin(), charVector.end());
    return charVector;
}

/*
 * formatLength(std::string inputString, int formatLength)
 * @param std::string inputString
 * @param int formatLength
 * 
 * @returns std::string - takes a std::string and formats it to the length specified in formatLength
*/
std::string AlphaNumericDisplayController::formatLength(std::string inputString, int formatLength) {
    if (inputString.length() < formatLength) {
        while (inputString.length() < formatLength) {
            inputString = "#" + inputString;
        }
    } else if (inputString.length() > formatLength) {
        inputString = inputString.substr(0, formatLength);
    }

    return inputString;
}






void AlphaNumericDisplayController::displayTest(std::string arduinoSetupName)
{
    //char testRunArr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '#', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    std::string testRunArr[37] = {"ab", "bc", "cd", "de", "ef", "fg", "gh", "hi", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "#", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};


   const int writeDataPin = getPinByNames(arduinoSetupName, "dataPin");
   const int writeClockPin = getPinByNames(arduinoSetupName, "clockPin");
   const int writeLatchPin = getPinByNames(arduinoSetupName, "latchPin");
    
    for (std::string testRunChar : testRunArr)
    {   
        const std::vector<char> formattedDisplayData = formatAndConcatenateDisplayData({{testRunChar, 2}});
        
        // Shift out the data to the shift register
        digitalWrite(writeLatchPin, LOW);
            writeDataToDisplay(formattedDisplayData, arduinoSetupName);
        digitalWrite(writeLatchPin, HIGH);

        delay(750);
    }
}


void AlphaNumericDisplayController::addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin) {
    std::vector<PinConfig> newPinConfig = {{"dataPin", newDataPin}, {"clockPin", newClockPin}, {"latchPin", newLatchPin}};
    arduinoSetup.push_back(std::make_pair(newSetupName, newPinConfig));
}

void AlphaNumericDisplayController::printArduinoSetup(){
    for (const auto& setup : arduinoSetup) {
        Serial.print("Device: ");
        Serial.println(setup.first.c_str()); // Convert C++ string to C-style string
        Serial.println("Pin Configuration:");
        for (const PinConfig& pin : setup.second) {
            Serial.print("  ");
            Serial.print(pin.name.c_str()); // Convert C++ string to C-style string
            Serial.print(": ");
            Serial.println(pin.value);
        }
        Serial.println();
    }
}


int AlphaNumericDisplayController::getPinByNames(const std::string& setupName, const std::string& pinName) {
    for (const auto& setup : arduinoSetup) {
        if (setup.first == setupName) {
            for(const PinConfig& pin : setup.second){
                if(pin.name == pinName){
                    return pin.value;
                }   
            }
        }
    }
}