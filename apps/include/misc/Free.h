//
// Created by ghisi on 19.03.23.
//

#ifndef AVR_FREE_H
#define AVR_FREE_H

#include "comms/Serial.h"
#include "cstdint"
#include "shell/CommandLine.h"
#include "system/OS.h"

class Free {
 public:
  static int_fast8_t run(char* args);
};

int_fast8_t Free::run(char* args) {
    OS::MemoryDump();
//  auto memoryStats = OS::memoryStats();
//  printf("Bytes\r\n");
//  printf(" Size:\t%u\r\n", memoryStats->size);
//  printf(" Total:\t%u\r\n", memoryStats->used + memoryStats->free);
//  printf(" Used:\t%u\r\n", memoryStats->used);
//  printf(" Free:\t%u\r\n", memoryStats->free);
//  printf("Blocks\r\n");
//  printf(" Total:\t%u\r\n", memoryStats->usedBlocks + memoryStats->freeBlocks);
//  printf(" Used:\t%u\r\n", memoryStats->usedBlocks);
//  printf(" Free:\t%u\r\n", memoryStats->freeBlocks);

  return 0;
}

#endif  // AVR_FREE_H
