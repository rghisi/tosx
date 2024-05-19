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
    static MemoryAllocator *memoryAllocator;
    static Kernel* kernel;

    static void schedule(Task *task);
    static void schedule(PeriodicTask *task);
    static void switchToTask(Task *task);
    static void yield();
    static void preempt();
    static void enablePreemption();
    static void disablePreemption();
    static Promise* await(Promise *promise);
    static void sleep(uint_fast16_t ms);
    static void *memalloc(size_t size);
    static void memfree(void *ptr);
    static Task* createTask(const char* name, int_fast8_t (*entryPoint)(char *), char *args);
    static MemoryStats *memoryStats();
    static void incrementTick();
    static uint32_t now();
    static void terminate(Task *task);
    static bool disableInterupts();
    static bool enableInterupts();
    static void enterCritical();
    static void leaveCritical();
};


#endif //AVR_OS_H
