//
// Created by ghisi on 10.09.22.
//
#include "std/Random.h"

RandomProvider *Random::provider = nullptr;

void Random::setup(RandomProvider *randomProvider) {
  Random::provider = randomProvider;
}

uint8_t Random::next() { return provider->next(); }

uint16_t Random::next16() { return provider->next16(); }

uint32_t Random::next32() { return provider->next32(); }

void Random::seed(uint8_t s) { provider->seed(s); }
