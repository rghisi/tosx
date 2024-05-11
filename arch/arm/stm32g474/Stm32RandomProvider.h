//
// Created by ghisi on 5/7/24.
//

#ifndef TOSX_STM32RANDOMPROVIDER_H
#define TOSX_STM32RANDOMPROVIDER_H

#include "std/RandomProvider.h"
#include <libopencm3/stm32/g4/rng.h>

class Stm32RandomProvider: public RandomProvider {
 public:
  Stm32RandomProvider();
  uint8_t next() override;
  uint16_t next16() override;
  uint32_t next32() override;
  void seed(uint32_t) override;
};

Stm32RandomProvider::Stm32RandomProvider() {
  rcc_periph_clock_enable(RCC_RNG);
// RNG_CR |= RNG_CR_CED;
//  RNG_CR |= RNG_CR_RNGEN;
  //rng_interrupt_enable();
  rng_enable();
}

uint8_t Stm32RandomProvider::next() {
  return (uint8_t) next32();
}

uint16_t Stm32RandomProvider::next16() {
  return (uint16_t) next32();
}

uint32_t Stm32RandomProvider::next32() {
  //auto newValue = RNG_DR;

  return rng_get_random_blocking();
}
void Stm32RandomProvider::seed(uint32_t) {}

#endif  // TOSX_STM32RANDOMPROVIDER_H
