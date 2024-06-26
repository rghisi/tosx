//
// Created by ghisi on 11.03.23.
//

#ifndef AVR_OS_H
#define AVR_OS_H

#include "MemoryAllocator.h"
#include "Event.h"
#include "Cpu.h"
#include "SysTicks.h"
#include "Kernel.h"

class OS {
public:
    static Kernel* kernel;

    static void schedule(Task *task);
    static void schedule(PeriodicTask *task);
    static void switchToTask(Task *task);
    static void yield();
    static void preempt();
    static Promise* await(Promise *promise);
    static void sleep(uint_fast16_t ms);
    static void *memalloc(size_t size);
    static void memfree(void *ptr);
    static Task* createTask(const char* name, int_fast8_t (*entryPoint)(char *), char *args);
    static MemoryStats *memoryStats();
    static void incrementTick();
    static uint32_t now();
    static void terminate(Task *task);
    static void PrintProcesses();

    static void MemoryDump();
};


#endif //AVR_OS_H
