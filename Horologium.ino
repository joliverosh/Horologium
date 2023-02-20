#include <ESP8266WiFi.h>
#include <WiFiManager.h> // --> https://github.com/tzapu/WiFiManager

#include <time.h>
const char* ntpServer = "pool.ntp.org";
char* TimeZ = "COT+5"; // Time zone "COT+5" http://famschmid.net/timezones.html

#include <TM1637Display.h> // Use the library included here
int pinClk = 0; // CLK signal in pin 0
int pinDIO = 2; // DIO signal in pin 2
TM1637Display display(pinClk, pinDIO);
const uint8_t colonMask = 0b01000000;

void setup() {
  // Set the display brightness 0-7
  display.setBrightness(0);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm; // global wm instance

  //reset WiFi settings
  //wm.resetSettings(); 

  // set dark theme
  wm.setClass("invert");
  wm.setConnectTimeout(180);
  bool res;
  
  // when the ESP module starts search for "Horologium" SSID network
  res = wm.autoConnect("Horologium");

  if (!res) {
    //delay(3000);
    //WiFi.disconnect(true);
    //Serial.println("Failed to connect");
    ESP.restart();
    delay(5000);
  }
  else {
    //if you get here you have connected to the WiFi    
    //Serial.println("WiFi connected");
  }
 
//__________________________________________________________

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", TimeZ , 1);
}

void loop() {
  time_t now = time(nullptr);
  struct tm * timeinfo;
  timeinfo = localtime(&now);

  //The blinking colon between hours amd minutes every even seconds 
  if ((timeinfo->tm_sec) % 2) {
    display.showNumberDecEx((timeinfo->tm_hour), colonMask, true, 2, 0);
  }
  else {
    display.showNumberDec((timeinfo->tm_hour), true, 2, 0);
  }
  display.showNumberDec((timeinfo->tm_min), true, 2, 2);
}
