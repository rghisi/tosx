//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_MEMTEST_H
#define TOSX_MEMTEST_H

#include "std/Random.h"
class MemTest {
 public:
  static int_fast8_t run(char* args);
  static int_fast8_t stressTest();
private:
    static uint8_t id;
};

uint8_t MemTest::id = 0;

int_fast8_t MemTest::run(char *args) {
  stressTest();

  return 0;
}

int_fast8_t MemTest::stressTest() {
  auto runId = ++MemTest::id;
  auto iterations = 10000000u;
  auto timestamp = OS::now();
  auto bytesWritten = 0u;
  auto bytesPerSecond = 0u;
  size_t limit = (OS::memoryStats()->free / 4);
  printf("%u -> Started s:%06u\r\n", runId, limit);
  for (auto iteration = 0lu; iteration < iterations; iteration++) {
    auto random = (size_t) Random::next16();
    while (random > limit) {
      random = (size_t) Random::next16();
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
      printf("%u -> d:%04lu i:%06lu s:%06u t:%08uB/s\r\n", runId, timeDelta, iteration, limit, bytesPerSecond);
    }
    limit = (OS::memoryStats()->free / 4);
  }

  printf("%u -> Finished\r\n", runId);
  return 0;
}

#endif  // TOSX_MEMTEST_H
