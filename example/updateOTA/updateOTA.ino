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

#define FIRMWARE_VERSION "0.1" // change for every version

int led_pin = 23;

const char *ssid = "YOUR_WIFI_SSID"; // Add WiFi SSID
const char *pass = "YOUR_WIFI_PASSWORD"; // Add WiFi password
constexpr int interval = 7000; // Wait for 7 seconds

const char* firmware_version = "http://192.168.5.142:3000/firmware/version";
const char* download_firmware = "http://192.168.5.142:3000/firmware/blink.bin";

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


  ota.checkAndPerformUpdate(FIRMWARE_VERSION, firmware_version, download_firmware);
  
  
}