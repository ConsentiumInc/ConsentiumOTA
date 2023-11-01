/***************************************************
  This is Consentium's OTA library
  ----> https://consentiuminc.online/
  Check out the links above for our tutorials and product diagrams.

  This Consentium's OTA library works only for ESP8266/ESP32/Raspberry Pi Pico W compatible Edge boards. 
  
  Written by Debjyoti Chowdhury for Consentium.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <ConsentiumOTA.h>

ConsentiumOTA ota;

const char *ssid = "YOUR_WIFI_SSID"; // Add WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD"; // Add WiFi password
constexpr int interval = 7000; // Wait for 7 seconds
const char* currentFirmwareVersion = "YOUR_FIRMWARE_VERSION"; // change for every version
const int updateInterval = 100; // Check for update after 100 cycles

int led_pin = 23;

int loopCounter = 0; // Counter to keep track of loop cycles

void setup() {
  ota.begin(); // Initialize IoT board
  ota.initWiFi(ssid, pass); // Begin WiFi connection

  pinMode(led_pin, OUTPUT);
}

void loop(){
  // basic blink code
  digitalWrite(led_pin, HIGH);
  delay(interval);
  digitalWrite(led_pin, LOW);
  delay(interval);

  
  loopCounter++;
  if (loopCounter >= updateInterval) {
    ota.checkAndPerformUpdate(currentFirmwareVersion);
    loopCounter = 0; // Reset the counter after checking for an update
  }
}
