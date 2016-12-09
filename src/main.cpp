#include <Arduino.h>
#include <stdarg.h>
#include <limits.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <NTPClient.h>

#include "main.hpp"
#include "WString.h"
#include "OTA.hpp"
#include "Network.hpp"
#include "fonts/Roboto.h"
#include "images/images.xcf.h"
#include "images/sprites.xcf.h"
#include "WeatherCondition.hpp"
#include "Logging.hpp"
#include "ThingSpeak.h"
#include "SH1106.h"
#include "OLEDDisplayUi.h"
#include "Weather.hpp"

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient client;

SH1106 display(0x3c, 12, 13);
OLEDDisplayUi ui ( &display );

WiFiUDP ntpUdp;
NTPClient timeClient(ntpUdp);

#define drawImage(display, x, y, name) display->drawXbm(x, y, IMG_##name##_width, IMG_##name##_height, IMG_##name##_bits)

WeatherCondition* currentWeather = new WeatherCondition();

int wifiRssi = 0;
void drawRssiImage(OLEDDisplay *display, int x, int y) {
    if(wifiRssi > -70) {
        drawImage(display, x, y, 4_BARS);
    } else if (wifiRssi > -80) {
        drawImage(display, x, y, 3_BARS);
    } else if (wifiRssi > -90) {
        drawImage(display, x, y, 2_BARS);
    } else if (wifiRssi > -100) {
        drawImage(display, x, y, 1_BAR);
    } else {
        // nothing
    }
}

void thingSpeakWrite(int channel, int value) {
    unsigned long myChannelNumber = 199884;
    const char * myWriteAPIKey = "GKH6ZHTXUBL57RTA";
    ThingSpeak.writeField(myChannelNumber, channel, value, myWriteAPIKey);
}

// Formats uptime
void uptimeString(char *buffer) {
    unsigned long secs = millis();
    unsigned long days = secs / ONE_DAY;
    unsigned long remainingSecs = secs - (days * ONE_DAY);
    unsigned long hours = remainingSecs / ONE_HOUR;
    remainingSecs = remainingSecs - (hours * ONE_HOUR);
    unsigned long minutes = remainingSecs / ONE_MINUTE;
    remainingSecs = remainingSecs - (minutes * ONE_MINUTE);

    if (days > 0) {
      sprintf(buffer, "%lu d, %lu h", days, hours);
    } else if (hours > 0){
      sprintf(buffer, "%lu h, %lu m", hours, minutes);
    } else {
      sprintf(buffer, "%lu m, %lu s", minutes, remainingSecs / 1000);
    }
}

void weatherInfo(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    char buffer[16];
    sprintf(buffer, "%d °C", currentWeather->temperature);
    display->setFont(Roboto_16);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(110+x, 18+y, buffer);
    drawImage(display, 112+x, 18+y, TEMPERATURE);

    buffer[16];
    sprintf(buffer, "%d %", round(currentWeather->humidity));
    display->drawString(110+x, 37+y, buffer);
    drawImage(display, 112+x, 37+y, HUMIDITY);
}

void heapOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    // overwrite complete area where text will be rendered
    display->setColor(BLACK); // alternate colors
    display->fillRect(0, 54, 128, 63);
    display->setColor(WHITE); // alternate colors

    char buffer[16];
    sprintf(buffer, "Heap: %u", ESP.getFreeHeap());
    display->setFont(Roboto_9);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128, 54, buffer);

    buffer[16];
    uptimeString(buffer);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0, 54, buffer);

    //display->drawString(10, 0, timeClient.getFormattedTime());
    //drawRssiImage(display, 0, 0);
}

void timeFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(Roboto_24);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128+x, 30+y, timeClient.getFormattedTime());
}

void airInfo(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setFont(Roboto_16);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);

    char buffer[16];
    sprintf(buffer, "%d m/s", round(currentWeather->windSpeed));
    display->drawString(110+x, 0+y, buffer);
    drawImage(display, 112+x, 0+y, WIND);

    buffer[16];
    sprintf(buffer, "%d °", round(currentWeather->windDirection));
    display->drawString(110+x, 18+y, buffer);
    drawImage(display, 112+x, 18+y, DIRECTION);

    buffer[16];
    sprintf(buffer, "%d hPa", round(currentWeather->pressure));
    display->drawString(110+x, 37+y, buffer);
    drawImage(display, 112+x, 37+y, HPA);
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { timeFrame, weatherInfo, airInfo };

// how many frames are there?
int frameCount = 3;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { heapOverlay };
int overlaysCount = 1;

void handleNetwork() {
    httpServer.handleClient();
    //httpServer.on('/index.html', handleIndex);
}

void setupNtp() {
  timeClient.setUpdateInterval(5 * ONE_MINUTE);
  timeClient.setTimeOffset( +(ONE_HOUR / 1000) );
  timeClient.begin();
}

void setupDisplay() {
  ui.setTargetFPS(30);

  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(LEFT);
  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_DOWN);
  ui.setFrames(frames, frameCount);
  ui.setOverlays(overlays, overlaysCount);
  //ui.disableAutoTransition();
  display.invertDisplay();

  ui.init();
  display.setContrast(0x50);
  display.flipScreenVertically();
}

void setup() {
  httpUpdater.setup(&httpServer);
  httpServer.begin();

  setupDisplay();

  Network::setup(&display, true);
  OTA::setup(&display);
  Logging::setup();

  ThingSpeak.begin(client);

  setupNtp();
}

unsigned long lastWeatherUpdate = LONG_MAX;
void updateWeather() {
  unsigned long updateInterval = 120000;
  if((millis() - lastWeatherUpdate) > updateInterval) {
    WeatherCondition *weather = getWeather(client);
    //getWeatherForecast(client);

    if(weather) {
      if (currentWeather) {
         delete currentWeather;
         currentWeather = NULL;
      }
      currentWeather = weather;
    }
    lastWeatherUpdate = millis();
  }
}

unsigned long lastDebugInfo = LONG_MAX;
void logDebugInfo() {
  unsigned long updateInterval = 60000;
  if((millis() - lastDebugInfo) > updateInterval) {
    char buffer[16];

    Logging::startStream();
    int freeHeap = ESP.getFreeHeap();
    sprintf(buffer, "Heap: %u \n", freeHeap);
    Logging::stream(buffer);

    buffer[16];
    uptimeString(buffer);
    Logging::stream(buffer);
    Logging::stream("\n");
    Logging::endStream();

    thingSpeakWrite(1, freeHeap);
    lastDebugInfo = millis();
  }
}

long lastWiFiRSSIUpdate = LONG_MAX;
void updateRssi() {
  unsigned long updateInterval = 1000;
  if(millis() - lastWiFiRSSIUpdate > updateInterval) {
    wifiRssi = WiFi.RSSI();
    lastWiFiRSSIUpdate = millis();
  }
}

void loop() {
  // Wait for an OTA update before doing anything else
  if(OTA::guard()) {
    OTA::handleUpdate();
    return;
  }

  handleNetwork();
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
    updateWeather();
    logDebugInfo();
    updateRssi();
    timeClient.update();

    delay(remainingTimeBudget);
  }
}
