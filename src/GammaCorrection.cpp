#include <stdarg.h>
#include "Logging.hpp"
#include "GammaCorrection.hpp"
#include "gamma.hpp"

uint8_t GammaCorrection::gamma[GAMMA_COLORS];

void GammaCorrection::setup(int bits) {
  float factor = (float)bits/(float)GAMMA_COLORS;
  for(int i=0; i < GAMMA_COLORS; i++) {
    GammaCorrection::gamma[i] = round(gamma_lookup[i]*factor);
  }
}

void GammaCorrection::printLookupTable() {
  uint8_t * g = GammaCorrection::gamma;

  Logging::startStream();
  Logging::stream("Gamma lookup table:\n");
  for(int i = 0; i < GAMMA_COLORS; i+=8) {
    char outstr[128];
    sprintf(outstr, "%d %d %d %d %d %d %d %d\n", g[i], g[i+1], g[i+2], g[i+3], g[i+4], g[i+5], g[i+6], g[i+7]);
    Logging::stream(outstr);
  }
  Logging::endStream();
}
