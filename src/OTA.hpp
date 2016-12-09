#include "SH1106.h"

#ifndef OTA_H
#define OTA_H

class OTA {
  public:
    static void setup(OLEDDisplay *display);
    static void handleUpdate();
    static bool guard();
  private:
    static bool guardHasBeenSatisfied;
    static unsigned long firstGuardExecution;

};

#endif
