//
// Created by ghisi on 10.09.22.
//

#ifndef AVR_RANDOM_H
#define AVR_RANDOM_H

#include <cstdint>

#include "RandomProvider.h"

class Random {
 protected:
  explicit Random(RandomProvider *provider);
  static Random *instance;

 public:
  Random(Random &other) = delete;
  void operator=(const Random &) = delete;

  static void setup(RandomProvider *randomProvider);
  static uint8_t next();
  static uint16_t next16();
  static uint32_t next32();
  static void seed(uint8_t);

 private:
  RandomProvider *provider;
};

#endif  // AVR_RANDOM_H
