void rgbBlink_0(int numerOfBlinks, int blinkSpeed, String colour){
    byte rgbColour[] = {LED_GREEN, LED_BLUE, LED_RED};
    int rgbCoulorLegnth = sizeof(rgbColour);
    byte rgbState[] ={LOW, HIGH};
    int rgbStateLegnth = sizeof(rgbState);

    int rgbColourStart = -1;
 if(colour == "M"){
    rgbColourStart = 0;
    rgbCoulorLegnth = 2;
 } else if(colour == "R"){
    rgbColourStart = 2;
 }else if(colour == "G"){
    rgbColourStart = 0;
    rgbCoulorLegnth = 1;
    }
 else if(colour == "B"){
    rgbColourStart = 1;
    rgbCoulorLegnth = 2;
    } 
 else{
    rgbColourStart = 0; 
 }     

for(int nob=0; nob<numerOfBlinks; nob++){
    for(int rgbC = rgbColourStart; rgbC<rgbCoulorLegnth; rgbC++){
        for(int rgbS = 0; rgbS<rgbStateLegnth; rgbS++){
            digitalWrite(rgbColour[rgbC], rgbState[rgbS]);
            delay(blinkSpeed);
        }
    }

}


