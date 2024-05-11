//
// Created by ghisi on 5/11/24.
//

#ifndef TOSX_SOFTWARERANDOMPROVIDER_H
#define TOSX_SOFTWARERANDOMPROVIDER_H

#include <cstdint>

#include "RandomProvider.h"

class SoftwareRandomProvider : public RandomProvider {
 public:
  uint8_t next() override;
  uint16_t next16() override;
  uint32_t next32() override;
  void seed(uint32_t) override;
 private:
  uint32_t _seed = 12345;
};

uint8_t SoftwareRandomProvider::next() {
  return (uint8_t) next32();
}

uint16_t SoftwareRandomProvider::next16() {
  return (uint16_t) next32();
}

uint32_t SoftwareRandomProvider::next32() {
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  uint32_t x = _seed;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return _seed = x;
}

void SoftwareRandomProvider::seed(uint32_t seed) {
  _seed = seed;
}

#endif  // TOSX_SOFTWARERANDOMPROVIDER_H
