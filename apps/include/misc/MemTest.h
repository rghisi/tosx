//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_MEMTEST_H
#define TOSX_MEMTEST_H

#include "std/Random.h"
class MemTest {
 public:
  static int_fast8_t run(char* args);
  static int_fast8_t staticTest(size_t freeMemory);
  static int_fast8_t stressTest(size_t freeMemory);
};

int_fast8_t MemTest::run(char *args) {
  auto freeMemory = OS::memoryStats()->free - 64;
  return stressTest(freeMemory);
}

int_fast8_t MemTest::stressTest(size_t freeMemory) {
  auto available = freeMemory - 128;
  auto iterations = 10000000;
  for (auto iteration = 0; iteration < iterations; iteration++) {
    printf("%d:", iteration);
    auto random = Random::next16();
    while (random >= available) {
      random = Random::next16();
    }
    printf("%u ", random);
    auto *array = new uint8_t[random];
    for (auto i = 0; i < random; i++) {
      array[i] = Random::next();
    }
    delete[] array;
    printf("\r\n");
  }

  return 0;
}

int_fast8_t MemTest::staticTest(size_t freeMemory) {
  auto bytes = freeMemory;
  auto halfWord = freeMemory / 2;
  auto word = freeMemory / 4;

  Serial::send("1 Byte\n\r");
  auto *array = new uint8_t[bytes];
  for (size_t i = 0; i < bytes; i++) {
    array[i] = (uint8_t) i;
    if (i % 1000 == 0 || i == bytes - 1) {
      Serial::send(i);
      Serial::send("\n\r");
    }
  }
  delete[] array;
  Serial::send("2 Bytes\n\r");
  auto *array2 = new uint16_t[halfWord];
  for (size_t i = 0; i < halfWord; i++) {
    array2[i] = (uint16_t) i;
    if (i % 1000 == 0 || i == halfWord - 1) {
      Serial::send(i);
      Serial::send("\n\r");
    }
  }
  delete[] array2;

  Serial::send("4 Bytes\n\r");
  auto *array3 = new uint32_t[word];
  for (size_t i = 0; i < word; i++) {
    array3[i] = i;
    if (i % 1000 == 0 || i == word - 1) {
      Serial::send(i);
      Serial::send("\n\r");
    }
  }
  delete[] array3;

  return 0;
}

#endif  // TOSX_MEMTEST_H
