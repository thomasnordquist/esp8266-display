#include "Network.hpp"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "ssid";
const char* password = "password";

void Network::setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}
