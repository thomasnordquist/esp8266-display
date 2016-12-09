#include "WeatherCondition.hpp"

WeatherCondition::WeatherCondition() {}
WeatherCondition::WeatherCondition(JsonObject& json) {
  bool hasValue = false;
  if(json != JsonObject::invalid()) {
    double kelvin = json["main"]["temp"];
    this->temperature = round(kelvin - 273.15);
    this->pressure = json["main"]["pressure"];
    this->humidity = json["main"]["humidity"];
    this->visibility = json["visibility"];
    this->windSpeed = json["wind"]["speed"];
    this->windDirection = json["wind"]["deg"];
    this->cloudiness = json["clouds"]["all"];
    this->isValid = true;
  }
}
