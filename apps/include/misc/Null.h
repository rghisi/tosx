//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_NULL_H
#define TOSX_NULL_H

#include <cstdint>

class Null {
 public:
  static int_fast8_t run(char* args);
};

int_fast8_t Null::run(char *args) {
  return 0;
}

#endif  // TOSX_NULL_H
