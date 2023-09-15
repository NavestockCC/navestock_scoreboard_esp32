# 1 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino"
# 2 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino" 2
# 3 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino" 2



AlphaNumericDisplayController anDispCont = AlphaNumericDisplayController();



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
    anDispCont.clearDisplay("Top", 2);
    anDispCont.clearDisplay("Bottom", 2);
}




void loop() {
    anDispCont.displayTest("Top", 2);
    anDispCont.displayTest("Bottom", 2);
}
