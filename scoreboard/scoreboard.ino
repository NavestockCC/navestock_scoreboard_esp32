

#include <Arduino.h>
#include "secrets.h"
#include "AlphaNumericDisplayController.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>   // Provide the token generation process info.
#include <addons/RTDBHelper.h>    // Provide the RTDB payload printing info and other helper functions.
#include <ArduinoJson.h> 



/**
 * Initialise Alpha Numeric Display Controller
 **/
    AlphaNumericDisplayController anDispCont = AlphaNumericDisplayController();
    const int bottomLength = 2;
    const int topLength = 2;
/**
 * Initialise Wifi
 **/
const String WIFI_SSID = SECRET_SSID;       //WiFi credentials SSID
const String WIFI_PASSWORD = SECRET_PASS;   //WiFi credentials Password

std::string wifiStatusToString(int status) {
  switch (status) {
    case WL_CONNECTED:
      return "Connected";
    case WL_IDLE_STATUS:
      return "Idle";
    case WL_NO_SSID_AVAIL:
      return "SSID";
    case WL_SCAN_COMPLETED:
      return "scan";
    case WL_CONNECT_FAILED:
      return "failed";
    case WL_CONNECTION_LOST:
      return "lost";
    case WL_DISCONNECTED:
      return "disconnected";
    default:
      return "error";
  }
}


/**
 * Initialise Firebase
 **/
const String API_KEY = SECRET_DATABASE_SECRET;          //RTDB API Key
const String USER_EMAIL = SECRET_USER_EMAIL;            //RTDB User email for authentication
const String USER_PASSWORD = SECRET_USER_PASSWORD;      //RTDB User password authentication

const String DATABASE_URL = SECRET_DATABASE_URL;        //RTDB URL
const String parentPath = SECRET_DATABASE_PARENT_PATH;  //RTDB Parent Path
const String childPath[2] = {"/Top", "/Bottom"};        //RTDB Child nodes to monitor

FirebaseJson scoreboardJSON;
FirebaseJsonData result;
FirebaseData fbdo;      //Firebase Data object
FirebaseData stream;    //Firebase RTDB Stream object

FirebaseAuth auth;        //Firebase Auth object
FirebaseConfig config;    //Firebase Config object

unsigned long sendDataPrevMillis = 0;

int count = 0;

volatile bool dataChanged = false;


void streamCallback(MultiPathStream stream)
{

  
   if (Firebase.RTDB.getJSON(&fbdo, "/Navestock_Scoreboard")) {
      scoreboardJSON = fbdo.to<FirebaseJson>();
      scoreboardJSON.get(result, "Top/Batsman1");
      const auto batsman1Value = result.to<String>();

      scoreboardJSON.get(result, "Top/Batsman2");
      const auto batsman2Value = result.to<String>();

      scoreboardJSON.get(result, "Top/Teamscore");
      const auto teamscoreValue = result.to<String>();

      scoreboardJSON.get(result, "Bottom/Overs");
      const auto oversValue = result.to<String>();

      scoreboardJSON.get(result, "Bottom/Wickets");
      const auto wicketsValue = result.to<String>();

      scoreboardJSON.get(result, "Top/Target");
      const auto targetValue = result.to<String>();

    
    anDispCont.writeDataToDisplay({{batsman1Value.c_str(), 1}, {batsman2Value.c_str(), 1}}, "Top"); //Display the value
    anDispCont.writeDataToDisplay({{oversValue.c_str(), 1}, {wicketsValue.c_str(), 1}}, "Bottom"); //Display the value

    /* Full Size Scoreboard
    anDispCont.writeDataToDisplay({{batsman1Value.c_str(), 3}, {teamscoreValue.c_str(), 3}, {batsman2Value.c_str(), 3}}, "Top"); //Display the value
    anDispCont.writeDataToDisplay({{oversValue.c_str(), 2}, {wicketsValue.c_str(), 1}, {targetValue.c_str(), 3}}, "Bottom"); //Display the value
    */

  } else {
    Serial.println("Failed to get JSON data");
  }
  

  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}

void setup(){

    /** Serial Monitor - Start **/
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


    /** Wifi - Start **/  
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
          while (WiFi.status() != WL_CONNECTED)
          {
            displayWiFiStatus();
          }
            displayWiFiStatus();

      Firebase.reconnectWiFi(true); // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
      //config.timeout.networkReconnect = 10 * 1000;    //Network reconnect timeout (interval) in ms (10 sec - 5 min) when network or WiFi disconnected.
      //config.timeout.sslHandshake = 2 * 60 * 1000;    //ESP32 SSL handshake in ms (1 sec - 2 min). This option doesn't allow in ESP8266 core library.

//TODO: look at this variable
      unsigned long ms = millis(); 


/** Firebase - Start **/
  config.api_key = API_KEY;             //Assign the api key (required)
  auth.user.email = USER_EMAIL;         //Assign the user sign in credentials
  auth.user.password = USER_PASSWORD;   //Assign the user sign in credentials
  config.database_url = DATABASE_URL;   //Assign the RTDB URL (required)

  config.token_status_callback = tokenStatusCallback; // Assign the callback function for the long running token generation task, see addons/TokenHelper.h
  config.timeout.socketConnection = 5 * 1000;         //Socket begin connection timeout (ESP32) or data transfer timeout (ESP8266) in ms (1 sec - 1 min).
  config.timeout.serverResponse = 10 * 1000;          //Server response read timeout in ms (1 sec - 1 min).
  config.timeout.rtdbKeepAlive = 45 * 1000;           //RTDB Stream keep-alive timeout in ms (20 sec - 2 min) when no server's keep-alive event data received.
  config.timeout.rtdbStreamReconnect = 1 * 1000;      //RTDB Stream reconnect timeout (interval) in ms (1 sec - 1 min) when RTDB Stream closed and want to resume.
  config.timeout.rtdbStreamError = 3 * 1000;          //RTDB Stream error notification timeout (interval) in ms (3 sec - 30 sec). It determines how often the readStream will return false (error) when it called repeatedly in loop.


  Firebase.begin(&config, &auth);
  stream.keepAlive(5, 5, 1);  // You can use TCP KeepAlive For more reliable stream operation and tracking the server connection status,  read https://github.com/mobizt/Firebase-ESP-Client#enable-tcp-keepalive-for-reliable-http-streaming

  if (!Firebase.RTDB.beginMultiPathStream(&stream, parentPath))
    Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());

  Firebase.RTDB.setMultiPathStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}

void loop()
{

  if (dataChanged){
    dataChanged = false;
}
}


/**
 * Wifi Utility Functions
 */
void displayWiFiStatus() {
  std::string wifiStatus = wifiStatusToString(WiFi.status());
  
  // Assuming 'anDispCont' and 'topLength' are defined elsewhere in your code
  anDispCont.writeDataToDisplay({{"wifi", topLength}}, "Top");
  anDispCont.writeDataToDisplay({{wifiStatus.c_str(), bottomLength}}, "Bottom");
}