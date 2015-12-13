#include <Arduino.h>
#include <FS.h>
#include "hspi/spi.c"
#include "Network.hpp"
#include "OTA.hpp"
#include "Logging.hpp"
#include "Color.hpp"
#include "GammaCorrection.hpp"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#define DEBUG 1
#define GPIO4 4
#define LED_COUNT 1024
#define COLOR_RESOLUTION 64 // in bits

Logging logging;
Color leds[LED_COUNT];

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setupMatrix(Color *led2s, int amount) {
  for(int i = 0; i<amount; i++) {
    Color *c = new Color();
    (*c).r = i%255;
    (*c).g = i % 3;
    (*c).b = i % 7;
    leds[i] = (*c);
  }
}

void setup() {
  Network::setup();
  Logging::setup();
  OTA::setup();
  setupMatrix(leds, LED_COUNT);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  spi_init(HSPI);
  spi_tx16_setup(HSPI, 0 /**/);

  pinMode(GPIO4, OUTPUT);   // Initialize the BUILTIN_LED pin as an output
  pinMode(5, OUTPUT);   // Initialize the BUILTIN_LED pin as an output

  GammaCorrection::setup(COLOR_RESOLUTION);
  #ifdef DEBUG
    GammaCorrection::printLookupTable();
  #endif
}

void handleNetwork() {
  OTA::handleUpdate();
  httpServer.handleClient();
}

#pragma GCC push_options
#pragma GCC optimize ("O2")
void writeToDisplay() {
  int cycles = 0;
  int line = 0x00;
  int columnCount = 0;
  for(int i = 0; i < LED_COUNT; i+=2) {
    //digitalWrite(5, HIGH);

    columnCount++;

    uint32_t output = 0x00;

    Color c = leds[i];

    if(c.r > cycles) {
      output |= 0x01;
    }

    if(c.g > cycles) {
      output |= (0x01 << 1);
    }

    if(c.b > cycles) {
      output |= (0x01 << 2);
    }

    c = leds[i+1];
    if(c.r > cycles) {
      output |= (0x01 << 3);
    }

    if(c.g > cycles) {
      output |= (0x01 << 4);
    }

    if(c.b > cycles) {
      output |= (0x01 << 5);
    }

    output |= line;

    if(columnCount == 64) {
      columnCount = 0;
      line+= 1 << 6;
    }

    output <<= 16;
    WRITE_PERI_REG(SPI_W0(HSPI), output);
    SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);

    //update clk pin
    output |= 0x01 << 11;
    WRITE_PERI_REG(SPI_W0(HSPI), output);
    SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);

    cycles++;
    //digitalWrite(5, LOW);
  }

}

void loop() {
  handleNetwork();

  //SPI.beginTransaction(SPISettings(4000000UL, MSBFIRST, SPI_MODE0));

  digitalWrite(GPIO4, HIGH);
  writeToDisplay();
  digitalWrite(GPIO4, LOW);

  //SPI.endTransaction();

   // Turn the LED on (Note that LOW is the voltage level
  //delay(1);
}
