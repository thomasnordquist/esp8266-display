#include <WString.h>
#include <WiFiClient.h>
#include "../Logging.hpp"

#ifndef JOKE_PROVIDER
#define JOKE_PROVIDER

class JokeProvider {
public:
    /*
     * This function will be called over and over until a joke is available,
     * the method is supposed to be non-blocking and should finish as fast as possible.
     * We need this since we have an UI with animation but only a single core, we need to finish
     * each run in less then 15ms; ideally we can do each operation in ~1ms
     *
     * Example, each number marks one method execution:
     * 1. Open connection
     * 2. connection not established, do nothing
     * 3. connection established, read one line
     * 4. line is no json ?, read one more line
     * 5. still has the line, parse it. set jokeAvailable = true & free memory
     */

    JokeProvider() {
      Logging::log("Jokes: start wifi client");
      _client = new WiFiClient();
    }

    ~JokeProvider() {
      _client->stop();
      delete _client;
    }

    virtual bool fetchJoke() = 0;
    bool isJokeAvailable() {
      return _hasJoke;
    }
    String getJoke() {
      return _joke;
    };

protected:
    WiFiClient* _client = NULL;
    bool _hasJoke = false;
    String _joke;

};


#endif
