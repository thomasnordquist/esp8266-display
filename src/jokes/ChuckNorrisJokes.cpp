#include "JokeProvider.hpp"
#include "ChuckNorrisJokes.hpp"
#include <ArduinoJson.h>

bool ChuckNorrisJokes::connect(String host) {
    if (!_client->connect((char *)&host, 80)) {
        _client->stop();
        return false;
    } else {
       return true;
    }
}

bool ChuckNorrisJokes::sendRequest(String url, String host) {
  _client->print(String("GET ") + url +
    " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "Connection: close\r\n\r\n");

  _requestHasBeenSent = true;
  return true;
}

bool ChuckNorrisJokes::shouldReportFail() {
  _fails++;
  return _fails <= _maxFails;
}

bool ChuckNorrisJokes::handleData() {
  String line = _client->readStringUntil('\n');

  if(line[0] != '{') { // skip non json lines
    return shouldReportFail();
  }

  if (!parseJoke(line)) {
    return shouldReportFail();
  }

  return true;
}

bool ChuckNorrisJokes::parseJoke(String line) {
  DynamicJsonBuffer *jsonBuffer = new DynamicJsonBuffer(2048);
  JsonObject& root = jsonBuffer->parseObject(line);
  String joke = root["value"]["joke"];
  delete jsonBuffer;

  if(NULL != joke) {
    _joke = joke;
    _hasJoke = true;
    return true;
  }

  return false;
}

bool ChuckNorrisJokes::fetchJoke() {
  String host = "api.icndb.com";
  String url = "http://api.icndb.com/jokes/random";

  if(!_client->connected()) {
      return this->connect(host); // connect
  } else if (!_requestHasBeenSent){
      return sendRequest(url, host); // send http request
  } else if (_client->connected()) {
    if (_client->available()) {
      bool data = handleData(); // handle the data
      if(isJokeAvailable()) {
        _client->stop();   // stop client if joke is received
      }
      return isJokeAvailable() || data;
    } else {
      return shouldReportFail(); // report that nothing has been done
    }
  } else {
    return shouldReportFail(); // report that nothing has been done
  }

}
