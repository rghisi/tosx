//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_MEMTEST_H
#define TOSX_MEMTEST_H

#include "std/Random.h"
class MemTest {
 public:
  static int_fast8_t run(char* args);
  static int_fast8_t stressTest(size_t limit);
};

int_fast8_t MemTest::run(char *args) {
  size_t limit = (OS::memoryStats()->free / 3) * 2;
  stressTest(limit);

  return 0;
}

int_fast8_t MemTest::stressTest(size_t limit) {
  auto iterations = 10000000u;
  auto timestamp = OS::now();
  auto bytesWritten = 0u;
  auto bytesPerSecond = 0u;
  for (auto iteration = 0lu; iteration < iterations; iteration++) {
    auto random = (uint32_t) Random::next16();
    while (random >= limit) {
      random = Random::next16();
    }
    auto *array = new uint8_t[random];
    for (auto i = 0; i < random; i++) {
      array[i] = (uint8_t) i;
    }
    delete[] array;

    bytesWritten += random;
    auto now = OS::now();
    auto timeDelta = now - timestamp;
    if (timeDelta >= 1000) {
      bytesPerSecond = bytesWritten / timeDelta * 1000;
      bytesWritten = 0;
      timestamp = now;
      printf("%04lu %06lu: %010uB/s\r", timeDelta, iteration, bytesPerSecond);
    }
  }

  return 0;
}

#endif  // TOSX_MEMTEST_H
