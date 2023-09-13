#line 1 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino"
#include <Arduino.h>
#include "AlphaNumericDisplayController.h"



AlphaNumericDisplayController anDispCont = AlphaNumericDisplayController();



#line 10 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino"
void setup();
#line 32 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino"
void loop();
#line 10 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino"
void setup() {
    Serial.begin(115200);
    
  /**
  * Setup AlphaNumeric Display Controller
  * add Pin settings with addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin)
  * then run begin()
  **/
    anDispCont.addArduinoSetup("Top", 10, 11, 12);
    anDispCont.addArduinoSetup("Bottom", 2, 3, 4);
    byte siftRegPinDefined[7] = {6, 7, 8, 9, 10, 11, 12};
    anDispCont.addShiftRegisterPinSetup(siftRegPinDefined); 
    bool _begin = anDispCont.begin();
    while(!_begin){
        Serial.println("begin() failed. ERROR in arduinoSetup!!!!!!!!");
    }
    anDispCont.printArduinoSetup();
}




void loop() {
    anDispCont.displayTest("Top", 2);
    anDispCont.displayTest("Bottom", 2);
}


