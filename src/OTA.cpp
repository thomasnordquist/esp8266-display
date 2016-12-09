#include "OTA.hpp"
#include "main.hpp"
#include "fonts/Roboto.h"
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include "SH1106.h"

#define OTA_GUARD_WAIT 1000
unsigned long OTA::firstGuardExecution = 0;
bool OTA::guardHasBeenSatisfied = false;

void OTA::setup(OLEDDisplay *display){
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("esp-display");

  // No authentication by default
  //ArduinoOTA.setPassword((const char *)"bierbier");

  ArduinoOTA.onStart([display]() {
    Serial.println("\nOTA Update");
    display->clear();
    display->setFont(Roboto_10);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 - 10, "OTA Update");
  });

  ArduinoOTA.onEnd([display]() {
    Serial.println("\nEnd");
    display->clear();
    display->setFont(Roboto_16);
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->drawString(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, "Flashing...");
    display->display();
  });

  ArduinoOTA.onProgress([display](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    display->drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
    display->display();
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void OTA::handleUpdate() {
  ArduinoOTA.handle();
}

bool OTA::guard() {
  if (OTA::guardHasBeenSatisfied) {
    return false;
  } else if (OTA::firstGuardExecution == 0) {
    OTA::firstGuardExecution = millis(); // initialize guard
    return true;
  }

  if (millis() - OTA::firstGuardExecution > OTA_GUARD_WAIT) {
    OTA::guardHasBeenSatisfied = true;
    return true;
  }

  return true;
}
