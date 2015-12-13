#include "main.hpp"
#include <Arduino.h>
#include <FS.h>
#include "Network.hpp"
#include "OTA.hpp"
#include "Logging.hpp"
#include "LedMatrix.hpp"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

Logging logging;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
  Network::setup();
  Logging::setup();
  OTA::setup();
  Logging::setup();
  LedMatrix::init();

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  pinMode(GPIO4, OUTPUT);   // Initialize the BUILTIN_LED pin as an output
  pinMode(5, OUTPUT);   // Initialize the BUILTIN_LED pin as an output
}

void handleNetwork() {
  OTA::handleUpdate();
  httpServer.handleClient();
}

void loop() {
  handleNetwork();

  digitalWrite(GPIO4, HIGH);
  LedMatrix::writeFrame();
  digitalWrite(GPIO4, LOW);
}
