#include <Arduino.h>
#include "gamma.hpp"

#ifndef GAMMA_CORRECTION_H
#define GAMMA_CORRECTION_H

class GammaCorrection {
  public:
    static void setup(int bits);
    static void printLookupTable();
    static uint8_t gamma[GAMMA_COLORS];
};

#endif
