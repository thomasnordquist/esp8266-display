#include "WeatherCondition.hpp"
#include <WiFiClient.h>

#ifndef WEATHER
#define WEATHER
#include <ESP8266WiFi.h>

WeatherCondition* getWeather(WiFiClient client);
WeatherCondition* getWeatherForecast(WiFiClient client);

#endif
