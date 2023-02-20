#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
// Module connection pins (Digital Pins)
//#define pinClk 0 //D3 D6 GPIO0
//#define pinDIO 2 //D4 D7 GPIO2
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
// Module connection pins (Digital Pins)
//#define pinClk 15
//#define pinDIO 23
#endif

#include <WiFiManager.h> // --> https://github.com/tzapu/WiFiManager

#include <time.h>
const char* ntpServer = "pool.ntp.org";
char* TimeZ = "COT+5"; // Time zone "COT+5" http://famschmid.net/timezones.html

#include <TM1637Display.h>
int pinClk = 0;
int pinDIO = 2;
TM1637Display display(pinClk, pinDIO);
const uint8_t colonMask = 0b01000000;


//#define ssid       "Jose"
//#define password    "oliveros1234"



void setup() {
  // Set the display brightness 0-7
  display.setBrightness(0);
/*
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    // Selectively set different digits
    display.showNumberDecEx(0, colonMask, true, 2, 0);
    display.showNumberDec(0, true, 2, 2);
    delay(500);
  }
*/

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm; // global wm instance

  //reset WiFi settings
  //wm.resetSettings(); 

  // set dark theme
  wm.setClass("invert");
  wm.setConnectTimeout(180);
  bool res;
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
 // String time = String(ctime(&now));
  struct tm * timeinfo;
  timeinfo = localtime(&now);

  //Los dos puntos entre las horas y los minutos titilando
  if ((timeinfo->tm_sec) % 2) {
    display.showNumberDecEx((timeinfo->tm_hour), colonMask, true, 2, 0);
  }
  else {
    display.showNumberDec((timeinfo->tm_hour), true, 2, 0);
  }
  display.showNumberDec((timeinfo->tm_min), true, 2, 2);
}
