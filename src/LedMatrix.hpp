#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <Arduino.h>
#include "Color.hpp"
class LedMatrix {
  public:
    static void init();
    static void writeFrame();
    static void handleUpdate();

  protected:
    static void writeToSpi(uint32_t reg);
    static void setupMatrix(uint32_t *colors, int amount);
    static uint32_t *leds;

};

#endif
