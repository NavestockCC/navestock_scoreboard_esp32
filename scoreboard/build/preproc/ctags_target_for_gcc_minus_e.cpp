# 1 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino"
# 2 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino" 2
# 3 "/Users/lefrascoetzee/Documents/Arduino/navestock_scoreboard_esp32/scoreboard/scoreboard.ino" 2



AlphaNumericDisplayController anDispCont = AlphaNumericDisplayController();



void setup() {
    Serial.begin(115200);

  /*
  * Setup AlphaNumeric Display Controller
  * add Pin settings with addArduinoSetup(const std::string& newSetupName, int newDataPin, int newClockPin, int newLatchPin)
  * then run begin()
  */
    anDispCont.addArduinoSetup("Top", 10, 11, 12);
    anDispCont.addArduinoSetup("Bottom", 2, 3, 4);
    bool _begin = anDispCont.begin();
    while(!_begin){
        Serial.println("begin() failed. ERROR in arduinoSetup!!!!!!!!");
    }
    anDispCont.printArduinoSetup();
}




void loop() {
    anDispCont.displayTest("Top");
    anDispCont.displayTest("Bottom");
}
