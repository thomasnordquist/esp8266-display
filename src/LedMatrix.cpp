#include "main.hpp"
#include "LedMatrix.hpp"
#include "hspi/spi.c"
#include "Color.hpp"
#include "GammaCorrection.hpp"

#define XT_CLI __asm__("rsil a2, 3");
#define XT_STI __asm__("rsil a2, 0");

#define RED(color) (color & 0x000000FF)
#define GREEN(color) (color & 0x0000FF00) >> 8
#define BLUE(color) (color & 0x00FF0000) >> 16

uint32_t *LedMatrix::leds;

void LedMatrix::init() {
  spi_init(HSPI);
  spi_tx16_setup(HSPI, 0);

  LedMatrix::leds = new uint32_t[LED_COUNT];

  GammaCorrection::setup(COLOR_RESOLUTION);
  #ifdef DEBUG
    GammaCorrection::printLookupTable();
  #endif
}

#pragma GCC optimize ("O2")
inline ICACHE_RAM_ATTR __attribute__((always_inline)) void LedMatrix::writeToSpi(uint32_t reg) {
  WRITE_PERI_REG(SPI_W0(HSPI), reg);
}

#pragma GCC optimize ("O2")
inline ICACHE_RAM_ATTR __attribute__((always_inline)) void flushSpi() {
  SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);
}

#pragma GCC push_options
#pragma GCC optimize ("inline", "O2")
void ICACHE_RAM_ATTR LedMatrix::writeFrame() {
  uint32_t *leds = LedMatrix::leds;
  #if OVERCLOCK == 1
    XT_CLI // disable interrupts
    REG_SET_BIT(0x3ff00014, BIT(1)); // overclock
  #endif

  for (int cycle=0; cycle<COLOR_RESOLUTION; cycle++) {

    int line = 0x00;
    int pixel = -1;
    for(int row = 0; row < 16; row++) { // 16 -> rows / 2
      uint32_t row_mask = row<<6;
      for(int col = 0; col < 32; col++) { // columns
        uint32_t output = row_mask;

        uint32_t color = leds[pixel++];
        uint32_t color2 = leds[pixel++];

        if(RED(color) > cycle) {
          output |= 0x01;
        }

        if(GREEN(color) > cycle ) {
          output |= (0x01 << 1);
        }

        /*
         * output registery can be busy and block the cpu, I flush before I write to the output register.
         * it saves me 600us (12%) in my test configuration
         * This way I write one time to often but, the "dirt" should be shifted out immediatly afterwards
         * catching it with if(cycle > 0) adds another 100us (which is 2.5%)
         * benchmarks at my test setup show that this is the best position to flush; maybe I have to move it later
         */
        flushSpi();

        if(BLUE(color) > cycle ) {
          output |= (0x01 << 2);
        }

        if(RED(color2) > cycle) {
          output |= (0x01 << 3);
        }

        if(GREEN(color2) > cycle ) {
          output |= (0x01 << 4);
        }

        if(BLUE(color2) > cycle ) {
          output |= (0x01 << 5);
        }

        // writing data to shift register,
        // display_clk = !spi_enable (this way we can avoid writing to very slow registers)
        writeToSpi(output<<16);
      }
    }
  }
  flushSpi();
  #if OVERCLOCK == 1
    REG_SET_BIT(0x3ff00014, BIT(0)); // return to normal speed
    XT_STI // enable interrupts
  #endif
}

void LedMatrix::setupMatrix(uint32_t *colors, int amount) {
  for(int i = 0; i<amount; i++) {
    Color *c = new Color();
    (*c).r = i%255;
    (*c).g = 128;
    (*c).b = i % 7;
    uint32_t color = (*c).b;
    color |= (*c).g << 8;
    color |= (*c).b << 16;
    colors[i] = color;
  }
}
