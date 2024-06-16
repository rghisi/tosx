//
// Created by ghisi on 10.09.22.
//

#ifndef AVR_RANDOM_H
#define AVR_RANDOM_H

#include <cstdint>

#include "RandomProvider.h"

class Random {
 public:
  static void setup(RandomProvider *randomProvider);
  static uint8_t next();
  static uint16_t next16();
  static uint32_t next32();
  static void seed(uint8_t);

 private:
  static RandomProvider *provider;
};

#endif  // AVR_RANDOM_H
