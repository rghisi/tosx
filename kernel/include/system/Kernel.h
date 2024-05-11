//
// Created by ghisi on 21.03.24.
//

#ifndef AVR_KERNEL_H
#define AVR_KERNEL_H

#include "Cpu.h"
#include "MemoryAllocator.h"
#include "Scheduler.h"
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
  Promise *await(Promise *promise);
  void sleep(uint_fast16_t ms);
  void incrementTick();
  uint32_t now();
  bool disableInterrupts();
  bool enableInterrupts();
  void enterCritical();
  void leaveCritical();
  void killCurrentTask();

 private:
  IdleTask *idleTask;
  Scheduler *scheduler;
  Cpu *cpu;
  SysTicks *sysTicks;
  Task *currentTask;
  Task *runningTask;
  bool preemptionEnabled = false;
  bool interruptsEnabled = false;
  bool interruptsBeforeCritical = false;
  bool preemptionBeforeCritical = false;
  Task *criticalTask = nullptr;
  bool criticalAttempt = false;
  bool preemptionSuspended = false;
  void startKernelTask();
  void checkStackOverflow(Task *task);
  void reportMemory();

};

#endif  // AVR_KERNEL_H
