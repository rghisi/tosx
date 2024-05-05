//
// Created by ghisi on 19.03.23.
//

#ifndef AVR_FREE_H
#define AVR_FREE_H


#include "cstdint"
#include "system/OS.h"
#include "comms/Serial.h"
#include "shell/CommandLine.h"

class Free {
public:
    enum class Flag { HUMAN_READABLE, NONE } uint8_t;
    void reportMemory(Flag flag);
    static int_fast8_t run(char* args);
private:
    static size_t scale(size_t i, Flag flag);
};

void Free::reportMemory(Flag flag) {
    auto memoryStats = OS::memoryStats();
    auto size = memoryStats->size;
    auto total = scale(memoryStats->used + memoryStats->free, flag);
    auto used = scale(memoryStats->used, flag);
    auto free = scale(memoryStats->free, flag);
    auto totalBlocks = scale(memoryStats->usedBlocks + memoryStats->freeBlocks, flag);
    auto usedBlocks = scale(memoryStats->usedBlocks, flag);
    auto freeBlocks = scale(memoryStats->freeBlocks, flag);

    printf("Bytes\r\n");
    printf(" Size:\t%lu\r\n", size);
    printf(" Total:\t%lu\r\n", total);
    printf(" Used:\t%lu\r\n", used);
    printf(" Free:\t%lu\r\n", free);
    printf("Blocks\r\n");
    printf(" Total:\t%lu\r\n", totalBlocks);
    printf(" Used:\t%lu\r\n", usedBlocks);
    printf(" Free:\t%lu\r\n", freeBlocks);
}

int_fast8_t Free::run(char *args) {
    auto commandLine = new CommandLine(args);
    auto reporter = new Free();
    auto flag = Flag::NONE;
    if (commandLine->hasParameter("-h")) {
        flag = Flag::HUMAN_READABLE;
    }
    reporter->reportMemory(flag);

    delete commandLine;
    delete reporter;

    return 0;
}

size_t Free::scale(size_t value, Free::Flag flag) {
    if (flag == Flag::HUMAN_READABLE && value > 1024) {
        return value >> 10;
    }

    return value;
}

#endif //AVR_FREE_H
