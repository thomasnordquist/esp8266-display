#include "main.hpp"
#include "LedMatrix.hpp"
#include "hspi/spi.c"
#include "Color.hpp"
#include "GammaCorrection.hpp"

Color *LedMatrix::leds;

void LedMatrix::init() {
  spi_init(HSPI);
  spi_tx16_setup(HSPI, 0);

  LedMatrix::leds = new Color[LED_COUNT];

  GammaCorrection::setup(COLOR_RESOLUTION);
  #ifdef DEBUG
    GammaCorrection::printLookupTable();
  #endif
}

#pragma GCC optimize ("Os")
inline __attribute__((always_inline)) void LedMatrix::writeToSpi(uint32_t reg) {
  WRITE_PERI_REG(SPI_W0(HSPI), reg);
  SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);
}

#pragma GCC push_options
#pragma GCC optimize ("inline", "O2")
void LedMatrix::writeFrame() {
  Color *leds = LedMatrix::leds;
  for (int cycle=0; cycle<COLOR_RESOLUTION; cycle++) {

    int line = 0x00;
    int columnCount = 0;

    for(int i = 0; i < LED_COUNT; i+=2) {
      columnCount++;
      uint32_t output = 0x00;

      Color c = leds[i];

      if(c.r > cycle) {
        output |= 0x01;
      }

      if(c.g > cycle) {
        output |= (0x01 << 1);
      }

      if(c.b > cycle) {
        output |= (0x01 << 2);
      }

      c = leds[i+1];
      if(c.r > cycle) {
        output |= (0x01 << 3);
      }

      if(c.g > cycle) {
        output |= (0x01 << 4);
      }

      if(c.b > cycle) {
        output |= (0x01 << 5);
      }

      output |= line;

      if(columnCount == 64) {
        columnCount = 0;
        line+= 1 << 6;
      }

      output <<= 16;
      writeToSpi(output);

      //update clk pin
      output |= 0x01 << 11;
    }
  }
}

void LedMatrix::setupMatrix(Color *leds, int amount) {
  for(int i = 0; i<amount; i++) {
    Color *c = new Color();
    (*c).r = i%255;
    (*c).g = i % 3;
    (*c).b = i % 7;
    leds[i] = (*c);
  }
}
