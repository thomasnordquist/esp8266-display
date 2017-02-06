#ifndef CHUCK_NORRIS_JOKES
#define CHUCK_NORRIS_JOKES

#include "JokeProvider.hpp"
#include <ArduinoJson.h>

class ChuckNorrisJokes: public JokeProvider {
  public:
    bool fetchJoke();

  protected:
    bool connect(String host);
    bool sendRequest(String host, String url);
    bool handleData();
    bool parseJoke(String line);

    // this method decides if the connection/retreival has failed or to just retry within the next execution
    bool shouldReportFail();

    bool _requestHasBeenSent = false;
    int _fails = 0;
    int _maxFails = 200000;
};

#endif
