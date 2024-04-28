//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_MEMTEST_H
#define TOSX_MEMTEST_H

class MemTest {
 public:
  static int_fast8_t run(char* args);
};

int_fast8_t MemTest::run(char *args) {
  Serial::send("1 Byte\n\r");
  auto *array = new uint8_t[24000];
  for (size_t i = 0; i < 24000; i++) {
    array[i] = (uint8_t) i;
    if (i % 1000 == 0) {
      Serial::send(i);
      Serial::send("\n\r");
    }
  }
  delete[] array;
  Serial::send("2 Bytes\n\r");
  auto *array2 = new uint16_t[12000];
  for (size_t i = 0; i < 12000; i++) {
    array2[i] = (uint16_t) i;
    if (i % 1000 == 0) {
      Serial::send(i);
      Serial::send("\n\r");
    }
  }
  delete[] array2;

  Serial::send("4 Bytes\n\r");
  auto *array3 = new uint32_t[6000];
  for (size_t i = 0; i < 6000; i++) {
    array3[i] = i;
    if (i % 1000 == 0) {
      Serial::send(i);
      Serial::send("\n\r");
    }
  }
  delete[] array3;
  return 0;
}

#endif  // TOSX_MEMTEST_H
