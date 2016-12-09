#include "SH1106.h"

#ifndef NETWORK_H
#define NETWORK_H

class Network {
  public:
    static void setup(OLEDDisplay *display, bool wait);
};

#endif
