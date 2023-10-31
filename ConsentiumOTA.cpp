#include <HTTPUpdate.h>
#include <ConsentiumOTA.h>

#include "utils/consentium_essentials.h"

WiFiClient client;

const char* getRemoteFirmwareVersion(const char* firmware_url) {
  HTTPClient http;
  http.begin(firmware_url);
  int httpCode = http.GET();
  static char versionBuffer[128]; // Assuming version won't exceed 128 characters
  if (httpCode == 200) {
    http.getString().toCharArray(versionBuffer, sizeof(versionBuffer));
    return versionBuffer;
  } else {
    return nullptr; // Return nullptr on error
  }
}

ConsentiumOTA::ConsentiumOTA() {}

void ConsentiumOTA::begin() {
  Serial.begin(115200);
}

void ConsentiumOTA::initWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);

  Serial.print(F("Attempting to connect SSID: "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print(F("Got IP : "));
  Serial.println(WiFi.localIP());
}

void ConsentiumOTA::checkAndPerformUpdate(const char* deviceVersion) {
  const char* remoteVersion = getRemoteFirmwareVersion(versionURL);

  Serial.print(F("Remote version: "));
  Serial.print(remoteVersion);
  Serial.print(F(" On-device version: "));
  Serial.print(deviceVersion);

  if (remoteVersion && strcmp(remoteVersion, deviceVersion) != 0) {
    httpUpdate.rebootOnUpdate(false); // Remove automatic update

    Serial.println(F("-> Update available."));

    t_httpUpdate_return ret = httpUpdate.update(client, firmwareURL);

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        Serial.println(F("Retry in 10 secs!"));
        delay(10000); // Wait 10 secs before retrying
        break;

      case HTTP_UPDATE_OK:
        Serial.println(F("Updated downloaded"));
        delay(1000); // Wait a second and restart
        Serial.println(F("Restarting!"));
        ESP.restart();
        break;
    }
  } else {
    Serial.println("-> No update.");
  }
}
