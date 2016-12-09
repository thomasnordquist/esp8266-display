#include "Weather.hpp"
#include "Logging.hpp"
#include <ESP8266WiFi.h>

#define CITY 64283
#define COUNTRY_CODE de
#define API_KEY cc800e6c32dc5ee4d2feacdbb2800305

const char host[] = "api.openweathermap.org";
const char weatherUrl[] = "http://api.openweathermap.org/data/2.5/weather?q=64283,de&appid=cc800e6c32dc5ee4d2feacdbb2800305";
const char forecastUrl[] = "http://api.openweathermap.org/data/2.5/forecast?q=64283,de&appid=cc800e6c32dc5ee4d2feacdbb2800305";

// {"coord":{"lon":8.65,"lat":49.87},"weather":[{"id":701,"main":"Mist","description":"mist","icon":"50d"}],"base":"stations","main":{"temp":272.15,"pressure":1028,"humidity":80,"temp_min":271.15,"temp_max":273.15},"visibility":8000,"wind":{"speed":1.5,"deg":240},"clouds":{"all":90},"dt":1480940400,"sys":{"type":1,"id":4881,"message":0.0032,"country":"DE","sunrise":1480921658,"sunset":1480951497},"id":2938913,"name":"Darmstadt","cod":200}HTTP/1.1 200 OK
WeatherCondition* getWeather(WiFiClient client) {

  if (!client.connect(host, 80)) {
      Serial.println("connection failed");
      client.stop();
      return NULL;
  }

  client.print(String("GET ") + weatherUrl + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

  while(client.connected()) {
    while(client.available()){
       String line = client.readStringUntil('\n');

       if(line[0] != '{') { // skip non json lines
         continue;
       }

       DynamicJsonBuffer *jsonBuffer = new DynamicJsonBuffer(2048);

       JsonObject& root = jsonBuffer->parseObject(line);
       WeatherCondition *condition = new WeatherCondition(root);

       client.stop();
       delete jsonBuffer;

       if(condition->isValid) {
         return condition;
       } else {
         delete condition;
         return NULL;
       }
    }
  }

  client.stop();
  return NULL;
}

WeatherCondition* getWeatherForecast(WiFiClient client) {

  int freeHeap = ESP.getFreeHeap();
  char buffer[32];
  sprintf(buffer, "Heap: %u \n", freeHeap);
  Logging::log(buffer);

  if (!client.connect(host, 80)) {
      Serial.println("connection failed");
      client.stop();
      return NULL;
  }

  client.print(String("GET ") + forecastUrl + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

  while(client.connected()) {
    while(client.available()){
       String line = client.readStringUntil('\n');

       if(line[0] != '{') { // skip non json lines
         continue;
       }

       DynamicJsonBuffer *jsonBuffer = new DynamicJsonBuffer(4096);

       JsonObject& root = jsonBuffer->parseObject(line);
       JsonArray& forecast = root["list"];
       for(JsonArray::iterator it=forecast.begin(); it!=forecast.end(); ++it)
       {
          JsonObject& value = *it;
          WeatherCondition *condition = new WeatherCondition(value);
       }

       int freeHeap = ESP.getFreeHeap();
       buffer[32];
       sprintf(buffer, "Heap: %u \n", freeHeap);
       Logging::log(buffer);

      client.stop();
      delete jsonBuffer;
    }
  }

  client.stop();
  return NULL;
}
