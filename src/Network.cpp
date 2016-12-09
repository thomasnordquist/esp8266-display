#include "fonts/Roboto.h"
#include "main.hpp"
#include "Network.hpp"

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = AP_SSID;
const char* password = AP_PASSWORD;

void tryConnect(OLEDDisplay *display) {
  display->clear();
  display->setFont(Roboto_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(64, 16, "Connecting");
  display->setFont(Roboto_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawStringMaxWidth(0, 30, 128, ssid);
  display->drawStringMaxWidth(0, 50, 128, password);
  display->display();
}

void connectionCompleted(OLEDDisplay *display) {
  display->clear();
  display->setFont(Roboto_16);
  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(64, 16, "Connected");
  IPAddress ip = WiFi.localIP();

  display->drawString(64, 32, ip.toString());
  display->display();
}

void Network::setup(OLEDDisplay *display, bool wait = true) {
  Serial.begin(115200);
  Serial.println("Connecting");
  tryConnect(display);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (wait && WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    display->drawString(0, 44, "Connection Failed, reboot");
    display->display();
    delay(5000);
    ESP.restart();
  }

  connectionCompleted(display);
}
