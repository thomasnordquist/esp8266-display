#include "main.hpp"
#include "LedMatrix.hpp"
#include "hspi/spi.c"
#include "Color.hpp"
#include "GammaCorrection.hpp"

#define XT_CLI __asm__("rsil a2, 3");
#define XT_STI __asm__("rsil a2, 0");

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
inline ICACHE_RAM_ATTR __attribute__((always_inline)) void LedMatrix::writeToSpi(uint32_t reg) {
  WRITE_PERI_REG(SPI_W0(HSPI), reg);
  SET_PERI_REG_MASK(SPI_CMD(HSPI), SPI_USR);
}

#pragma GCC push_options
#pragma GCC optimize ("inline", "O2")
void ICACHE_RAM_ATTR LedMatrix::writeFrame() {
  Color *leds = LedMatrix::leds;
  for (int cycle=0; cycle<COLOR_RESOLUTION; cycle++) {
    #if OVERCLOCK == 1
      XT_CLI // disable interrupts
      REG_SET_BIT(0x3ff00014, BIT(1)); // overclock
    #endif

    int line = 0x00;
    int pixel = -1;
    for(int row = 0; row < 16; row++) { // 16 -> rows / 2
      for(int col = 0; col < 32; col++) { // columns
        uint32_t output = row<<6;

        pixel++;
        Color c = leds[pixel];

        if(c.r > cycle) {
          output |= 0x01;
        }

        if(c.g > cycle) {
          output |= (0x01 << 1);
        }

        if(c.b > cycle) {
          output |= (0x01 << 2);
        }

        pixel++;
        c = leds[pixel];
        if(c.r > cycle) {
          output |= (0x01 << 3);
        }

        if(c.g > cycle) {
          output |= (0x01 << 4);
        }

        if(c.b > cycle) {
          output |= (0x01 << 5);
        }

        // writing data to shift register,
        // display_clk = !spi_enable (this way we can avoid writing to very slow registers)
        writeToSpi(output<<16);
      }
    }
    #if OVERCLOCK == 1
      REG_SET_BIT(0x3ff00014, BIT(0)); // return to normal speed
      XT_STI // enable interrupts
    #endif
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
