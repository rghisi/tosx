//
// Created by ghisi on 21.03.24.
//

#ifndef AVR_KERNEL_H
#define AVR_KERNEL_H

#include "Scheduler.h"
#include "MemoryAllocator.h"
#include "Cpu.h"
#include "SysTicks.h"

class Kernel {
public:
    Kernel(Cpu *cpu, Scheduler *scheduler);
    void start();
    void schedule(Task *task);
    void schedule(PeriodicTask *periodicTask);
    void switchToTask(Task *task);
    void terminate(Task *task);
    void yield();
    void preempt();
    void enablePreemption();
    void disablePreemption();
    Promise* await(Promise *promise);
    void sleep(uint_fast16_t ms);
    void incrementTick();
    uint32_t now();
    bool disableInterrupts();
    bool enableInterrupts();
    bool isInterruptsEnabled();

   private:
    IdleTask *idleTask;
    Scheduler *scheduler;
    Cpu *cpu;
    SysTicks *sysTicks;
    bool preemptionEnabled;
    Task* currentTask;
    bool interruptsEnabled;
    void checkStackOverflow(Task *task);
};


#endif //AVR_KERNEL_H
