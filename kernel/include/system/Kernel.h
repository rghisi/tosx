//
// Created by ghisi on 21.03.24.
//

#ifndef AVR_KERNEL_H
#define AVR_KERNEL_H

#include <list>
#include "Cpu.h"
#include "MemoryAllocator.h"
#include "Scheduler.h"
#include "SysTicks.h"
#include "Mutex.h"

class Kernel {
public:
    Kernel(Cpu *cpu, Scheduler *scheduler, MemoryAllocator *memoryAllocator);
    void start();
    void schedule(Task *task);
    void schedule(PeriodicTask *periodicTask);
    void switchToTask(Task *task);
    void terminate(Task *task);
    void switchToScheduler();
    void preempt();
    Promise *await(Promise *promise);
    void sleep(uint_fast16_t ms);
    void incrementTick();
    uint32_t now();

    void *allocateMemory(size_t size);
    void freeMemory(void *allocation);
    uint_fast16_t RunningTaskId();
    std::list<Task*> Tasks();
    Task *CreateTask(const char *name, int_fast8_t (*entryPoint)(char *), char *args);

    MemoryStats *GetMemoryStats();

    void PrintProcesses();

    void MemoryDump();

private:
    std::list<Task*> tasks;
    IdleTask *idleTask;
    Scheduler *scheduler;
    Cpu *cpu;
    MemoryAllocator *memoryAllocator;
    SysTicks *sysTicks;
    Task *currentTask;
    Task *runningTask;
    uint_fast16_t runningTaskId = 0;
    Mutex memoryAllocationMutex = Mutex();
    bool preemptionEnabled = false;
    bool interruptsEnabled = false;
    bool preemptionSuspended = false;

    void enablePreemption();
    bool enableInterrupts();
    void startKernelTask();
    void checkStackOverflow(Task *task);
    void reportMemory();
};

#endif  // AVR_KERNEL_H
