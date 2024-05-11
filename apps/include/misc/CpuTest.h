//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_CPUTEST_H
#define TOSX_CPUTEST_H

class CpuTest {
 public:
  static int_fast8_t run(char* args);
};

int_fast8_t CpuTest::run(char *args) {
  auto iterations = 1000000000u;
  for (auto i = 0u; i < iterations; i++) {
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");
    if (i % 10000000u == 0) {
      printf("CPU %u\r\n", i);
    }
  }
  return 0;
}

#endif  // TOSX_CPUTEST_H
