//
// Created by ghisi on 5/7/24.
//

#ifndef TOSX_RANDOMPROVIDER_H
#define TOSX_RANDOMPROVIDER_H

#include <cstdint>

class RandomProvider {
 public:
  virtual uint8_t next() = 0;
  virtual uint16_t next16() = 0;
  virtual uint32_t next32() = 0;
  virtual void seed(uint8_t) = 0;
};

/*class NullRandomProvider : public RandomProvider {
 public:
  uint8_t next() override { return 0; }
  uint16_t next16() override { return 0; }
  uint32_t next32() override { return 0; }
  void seed(uint8_t) override {}
};*/

#endif  // TOSX_RANDOMPROVIDER_H
