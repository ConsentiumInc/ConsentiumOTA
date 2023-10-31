#include <HTTPUpdate.h>
#include <ConsentiumOTA.h>

WiFiClient client;

String getRemoteFirmwareVersion(const char* firware_url) {
  HTTPClient http;
  http.begin(firware_url); // Replace with the URL for the version file
  int httpCode = http.GET();
  if (httpCode == 200) {
    return http.getString();
  } else {
    return ""; // Return empty string on error
  }
}

ConsentiumOTA::ConsentiumOTA() {}

void ConsentiumOTA::begin(){
    Serial.begin(115200);
    Serial.println();
    Serial.println();
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

void ConsentiumOTA::checkAndPerformUpdate(String firware_version, const char* f_url, const char* u_url){  
  String remoteVersion = getRemoteFirmwareVersion(f_url);

  Serial.print("Remote version: ");
  Serial.print(remoteVersion);
  Serial.print(" On-device version: ");
  Serial.print(firware_version);

  
  if (remoteVersion != firware_version) {
      httpUpdate.rebootOnUpdate(false); // remove automatic update

      Serial.println(F("->Update available."));

      t_httpUpdate_return ret = httpUpdate.update(client, u_url);

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
          Serial.println(F("Retry in 10 secs!"));
          delay(10000); // Wait 10 secs before retrying
          break;

        case HTTP_UPDATE_OK:
          Serial.println("HTTP_UPDATE_OK");
          delay(1000); // Wait a second and restart
          ESP.restart();
          break;
      }
    } 
    else {
      Serial.println("->No upadte.");
    }
}