#line 1 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/AlphaNumericDisplayController.cpp"
#include "AlphaNumericDisplayController.h"

/***************************************************************************************
 * Variables to store the Arduino Config as a vector
 * std::vector<std::pair<std::string, std::vector<PinConfig>>>
 * 
 * {{"std::string setupName"},
 *  {
 *  {"std::sting dataPin", int pinModeValue},
 *  {"std::string clockPin"}, int pinModeValue},
 *  {"std::string latchPin"}, int pinModeValue}
 * }}
 * 
 * public methods:
 * - to add config - addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin)
 * - to print to serial port config - printArduinoSetup()
 * 
 * private methods:
 * - to get config - getPinByNames(const std::string& setupName, const std::string& pinName)
 * 
 * notes:
 *  - add arduino setup before running begin().
 ****************************************************************************************/
struct PinConfig {
    std::string name;
    int value;
};
std::vector<std::pair<std::string, std::vector<PinConfig>>> arduinoSetup = {};


/**
 * Add Arduino Pin Configuration
 * @param const std::string& newSetupName - configuration name which will be used to retrieve the config in the getter function.
 * @param int newDataPin - dataPin / DP / SER / DS
 * @param int newClockPin - clockPin  / SRCLK / SH_CP
 * @param int newLatchPin - latchPin / RCLK / ST_CP
 * 
 * @returns void
**/
void AlphaNumericDisplayController::addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin) {
    std::vector<PinConfig> newPinConfig = {{"dataPin", newDataPin}, {"clockPin", newClockPin}, {"latchPin", newLatchPin}};
    arduinoSetup.push_back(std::make_pair(newSetupName, newPinConfig));
    }

/**
 * Returns the OUTPUT Pin value given the SetupName and PinName
 * @param  const std::string& setupName - Setup name as defined with addArduinoSetup function
 * @param const std::string& pinName - Name of the OUTPUT Pin valid input are "dataPin", "clockPin", "latchPin"
 * @returns int - value of the OUTPUT Pin
**/
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

/**
 * Utility function to print current Arduino Setup to the Serial port
 * @returns void
**/
void AlphaNumericDisplayController::printArduinoSetup(){
    for (const auto& setup : arduinoSetup) {
        Serial.print("Device: ");
        Serial.println(setup.first.c_str()); 
        Serial.println("Pin Configuration:");
        for (const PinConfig& pin : setup.second) {
            Serial.print("  ");
            Serial.print(pin.name.c_str()); 
            Serial.print(": ");
            Serial.println(pin.value);
        }
        Serial.println();
    }
}





/***************************************************************************************
 * Variable to store the OUTPUT pin setup for the Shift Register
 * 
 * Public methods:
 * - addShiftRegisterPinSetup(byte pinArray[7]) - Adds OUTPUT pin configuration
 * 
 * Private methods:
 * - getShiftRegisterPins() - Retrieves the OUTPUT pin configuration
 ***************************************************************************************/

byte segmentPins[7] = {6, 7, 8, 9, 10, 11, 12}; // Default pin configuration

/**
 * Add OUTPUT pin config for the shift registers
 * @param pinArray[7] - Array with OUTPUT pin numbers
 * @return void
**/
void AlphaNumericDisplayController::addShiftRegisterPinSetup(byte pinArray[7]) {
    memcpy(segmentPins, pinArray, sizeof(segmentPins)); // Copy pinArray to segmentPins
}

/**
 * Get Shift Register pin definition
 * @returns byte[7] - Pins defined as OUTPUT pins for the shift register.
 * @returns If NO OUTPUT Pins are defined, the function will return the default value {6, 7, 8, 9, 10, 11, 12}
**/
byte* AlphaNumericDisplayController::getShiftRegisterPins() {
    if (segmentPins[0] == 0) {
        Serial.println("Error: segmentPins array is empty.");
        Serial.println("Setting to default value {6, 7, 8, 9, 10, 11, 12}. Please initialize it before calling begin.");
        byte siftRegPinDefined[7] = {6, 7, 8, 9, 10, 11, 12};
        addShiftRegisterPinSetup(siftRegPinDefined);  
    } 

    return segmentPins;
}








/***************************************************************************************
* Alpha Numeric display byte definitions 

          B00000001
            aaa
B00100000  f   b B00000010
           f   b
B01000000   ggg
           e   c
B00010000  e   c B00000100
            ddd
          B00001000


public methods:
* - get the byte value of the character - getDigitByte(const char input)
 ****************************************************************************************/
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

/**
 * returns the byte for the character from the digits array
 * @param const char input - valid characters from a-z or 0-9 all other characters will be converted to 0b00000000(represents a blank on the display)
 * @return byte - value to push to the shift register to creat the character on the 7Segment Display
*/
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








/**************************************************************
 *  AlphaNumericDisplayController Constructor
 **************************************************************/
AlphaNumericDisplayController::AlphaNumericDisplayController() {
    // Constructor implementation (if needed)
}



 /*******************************************************************************************************
 * Initialise the Arduino Output Pins and the ShiftRegister OUTPUT Pins
 * 
 * note:
 * before running begin setup, the Arduino and ShiftRegister config with
 * - addShiftRegisterPinSetup(byte pinArray[7])
 * - addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin)
 *******************************************************************************************************/
bool AlphaNumericDisplayController::begin(){

    // Validate that arduinoSetup vector is not empty
    if (arduinoSetup.empty()) {
        Serial.println("Error: arduinoSetup vector is empty. Please initialize it before calling begin.");
        return false; // Exit the function if validation fails
    }

    // get shift register pin OUTPUT pin definition 
    byte* _segmentPins = getShiftRegisterPins(); 
   

    // Set Arduino pins as outputs
        for (const auto& setup : arduinoSetup) {
            for (const PinConfig& pin : setup.second) {
                pinMode(pin.value, OUTPUT);
        }
    }

    // Set segment pins as outputs
        for (int i = 0; i < 7; i++)
        {
            pinMode(_segmentPins[i], OUTPUT);
        }

    return true;
}



/*******************************************************************************************************
 * Functions to send and format the data sent to the display
 * use the functions in combination to get the desired effect
 * 
 * public methods:
 * - writeDataToDisplay(const std::vector<char> dataToDisplay, std::string arduinoSetupName) - send data to the display
 * 
 * note:
 * There is a format to the data dataToDisplay
 * std::vector<std::pair<std::string, int>>
 * 
 * Example: For a display setup of OO O OOO wanting to display 5 3 123
 * {
 * {"5", 2},
 * {"3", 1}
 * {"123", 3}
 * }
*******************************************************************************************************/

/**
 * Write the dataToDisplay to the 7Segment Display
 * @param - const std::vector<std::pair<std::string, int>> dataToDisplay - the data to display
 * @param - std::string arduinoSetupName - name of the arduino setup to use
**/
void AlphaNumericDisplayController::writeDataToDisplay(const std::vector<std::pair<std::string, int>> dataToDisplay, std::string arduinoSetupName) {
   // Blink the RGB display Green and Blue to indicate writing data to display 
    rgbBlink(2, 25, {LED_GREEN, LED_BLUE});

//Format data ready for display    
    std::string formatResult;
    for (const auto pair : dataToDisplay) {
        const std::string str = pair.first;
        int length = pair.second;

        if (length >= 0 && str.length() > 0) {
            formatResult += formatLength(str, length);
        } else {
            // Handle invalid length (out of bounds)
            std::cerr << "Invalid length for string: " << str << std::endl;
            // You can choose to throw an exception or handle this error differently as needed.
        }
    }

    std::vector<char> charVectorToDisplay(formatResult.begin(), formatResult.end());
    std::reverse(charVectorToDisplay.begin(), charVectorToDisplay.end());
    

// Send data to display
    int _dataPin = getPinByNames(arduinoSetupName, "dataPin");
    int _clockPin = getPinByNames(arduinoSetupName, "clockPin");
    int _latchPin = getPinByNames(arduinoSetupName, "latchPin");

    // Shift out the data to the shift register
    digitalWrite(_latchPin, LOW);

    // Send a byte of data to shift registers in series, for each char in charVectorToDisplay 
    for (char dispChar : charVectorToDisplay) {
            byte digitToDisplay = getDigitByte(dispChar);

            for (int i = 7; i >= 0; i--) { // Iterate over the 8 bits of the character 
            digitalWrite(_dataPin, (digitToDisplay >> i) & 1);
            digitalWrite(_clockPin, HIGH);
            digitalWrite(_clockPin, LOW);
        }
    }
    digitalWrite(_latchPin, HIGH);

    // Blink RGB LED Red to indicate writeDataToDisplay completed
    rgbBlink(2, 50, {LED_RED});
}


/**
 * formatLength(std::string inputString, int formatLength)
 * @param std::string inputString
 * @param int formatLength
 * 
 * @returns std::string - takes a std::string and formats it to the length specified in formatLength
**/
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


/**
 * Utility function to send the a-z, blank and 0-9 to the display
 * @param - std::string arduinoSetupName - name of the arduino setup to display the data on
 * @param - int displayLength - number of 7Segment digits
*/
void AlphaNumericDisplayController::displayTest(std::string arduinoSetupName, int displayLength){
    std::string testRunArr[37] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "#", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};


    const int writeDataPin = getPinByNames(arduinoSetupName, "dataPin");
    const int writeClockPin = getPinByNames(arduinoSetupName, "clockPin");
    const int writeLatchPin = getPinByNames(arduinoSetupName, "latchPin");

        //Clear the display and set to blank
        clearDisplay(arduinoSetupName, displayLength);
        
        //Iterate over testRunArr Array and write data to the display
        int concatCounter = displayLength;
        std::string testRunString = "";
        for (std::string testRun : testRunArr){  
            if(testRunString.length() < displayLength){
                concatCounter--;
                testRunString = testRunString + testRun;
            }   else {
                writeDataToDisplay({{testRunString, displayLength}}, arduinoSetupName);
                delay(500);
                testRunString = testRun;
            }
        }
        //Write any remaining testRunArr Array data to the display
        if(testRunString.length() > 0){
            writeDataToDisplay({{testRunString, displayLength}}, arduinoSetupName);
            delay(500);
        }
        //Clear the display and set to blank
        clearDisplay(arduinoSetupName, displayLength);
    }

/**
 * Utility function to clear the display.
* @param - std::string arduinoSetupName - name of the arduino setup to display the data on
 * @param - int displayLength - number of 7Segment digits
 * @returns void
*/
void AlphaNumericDisplayController::clearDisplay(std::string arduinoSetupName, int displayLength){
    writeDataToDisplay({{"#", displayLength}}, arduinoSetupName);
    delay(500);
}


/****************************************************************************************************************
 * Utility function that blinks the RGB LED on the Arduino Nano
 * @param int numberOfBlinks - the number of times to blink the LED
 * @param int blinkSpeed - speed of the blinks in milliseconds
 * @param std::vector<byte> colour - the colour of the LED - only valid inputs are LED_GREEN, LED_BLUE, LED_RED
 * 
*****************************************************************************************************************/
void AlphaNumericDisplayController::rgbBlink(int numberOfBlinks, int blinkSpeed, std::vector<byte> colours){

   byte rgbState[2] ={LOW, HIGH};

   for(int nBlinks = 0; nBlinks < numberOfBlinks; nBlinks++){
      for(int ledState = 0; ledState < 2; ledState++){
         for(byte colour : colours ){
            digitalWrite(colour, rgbState[ledState]);
            delay(blinkSpeed);
         }
      }
   }
}