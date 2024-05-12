//
// Created by ghisi on 5/12/24.
//

#ifndef TOSX_RANDOMTEST_H
#define TOSX_RANDOMTEST_H

#include <cstdint>

class RandomTest {
 public:
  static int_fast8_t run(char* args);
};

int_fast8_t RandomTest::run(char* args) {
  for (auto i = 0u; i < 100; i++) {
    auto random = Random::next16();
    printf("%u:%u\r\n", i, random);
  }

  return 0;
}

#endif  // TOSX_RANDOMTEST_H
