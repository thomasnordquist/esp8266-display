#ifndef LOGGING_H
#define LOGGING_H
class Logging {
  public:
    static void setup();
    static void log(char* message);
    static void startStream();
    static void endStream();
    static void stream(char* s);
};

#endif
