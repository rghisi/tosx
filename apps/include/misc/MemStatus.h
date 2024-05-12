//
// Created by ghisi on 19.03.23.
//

#ifndef AVR_MEMSTATUS_H
#define AVR_MEMSTATUS_H

#include "cstdint"
#include "system/OS.h"

class MemStatus {
 public:
  [[noreturn]] static int_fast8_t run(char* args);
  static void reportMemory();
};

void MemStatus::reportMemory() {
  auto memoryStats = OS::memoryStats();
  auto totalSeconds = OS::now() / 1000;
  auto hours = (totalSeconds / 3600);
  auto minutes = (totalSeconds - (3600 * hours)) / 60;
  auto seconds = (totalSeconds - (3600 * hours) - (minutes * 60));
  printf("\033[s\033[H\033[2K\033[1;41m  T %06u  U %06u  F %06u  %02lu:%02lu:%02lu \033[u\033[0m",
         memoryStats->size,
         memoryStats->used,
         memoryStats->free,
         hours,
         minutes,
         seconds
 );
}

[[noreturn]] int_fast8_t MemStatus::run(char* args) {
  while (true) {
    reportMemory();
    OS::sleep(250);
  }
}

#endif  // AVR_MEMSTATUS_H
