#include <WString.h>
#include <ArduinoJson.h>

#ifndef WEATHER_CONDITION
#define WEATHER_CONDITION

class WeatherCondition {
public:
  WeatherCondition(JsonObject& json);
  WeatherCondition();
  bool isValid = false;
  long temperature;
  long pressure;
  long humidity;

  long visibility;
  long cloudiness;

  double windSpeed;
  long windDirection;

  bool valid = false;

  ~WeatherCondition() {
  }
};

#endif
