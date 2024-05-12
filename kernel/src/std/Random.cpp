//
// Created by ghisi on 10.09.22.
//
#include "std/Random.h"

Random *Random::instance = nullptr;

Random::Random(RandomProvider *provider) { this->provider = provider; }
void Random::setup(RandomProvider *randomProvider) {
  if (instance == nullptr) {
    instance = new Random(randomProvider);
  }
}

uint8_t Random::next() { return instance->provider->next(); }

uint16_t Random::next16() { return instance->provider->next16(); }

uint32_t Random::next32() { return instance->provider->next32(); }

void Random::seed(uint8_t s) { instance->provider->seed(s); }
