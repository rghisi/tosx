//
// Created by ghisi on 21.03.24.
//

#include "system/Kernel.h"

#include <cstdio>

#include "comms/Serial.h"
#include "system/TaskState.h"
#include "system/TimeWaitPromise.h"

Kernel::Kernel(Cpu *cpu, Scheduler *scheduler) {
  this->cpu = cpu;
  this->scheduler = scheduler;
}

void Kernel::start() {
  Serial::send("\n\rStarting OS\n\r");
  Serial::send("\tWall clock: ");
  sysTicks = new SysTicks();
  cpu->setupSysTicks();
  Serial::send("OK\n\r");
  Serial::send("\tEnabling Interrupts: ");
  cpu->enableInterrupts();
  interruptsEnabled = true;
  Serial::send("OK\n\r");
  Serial::send("\tEnabling Preemption: ");
  enablePreemption();
  Serial::send("OK\n\r");
  Serial::send("\tSetting up scheduler\n\r");
  idleTask = new IdleTask();
  cpu->initialize(idleTask);
  idleTask->ready();
  scheduler->setIdleTask(idleTask);
  cpu->initialize(scheduler);
  cpu->swapContext(nullptr, scheduler->stack->pointer);
  Serial::send("\n\rPanic! Kernel Task Ended\n\r");
  while (true);
}

void Kernel::schedule(Task *task) {
  if (task->isCreated()) {
    cpu->initialize(task);
    task->ready();
  }
  scheduler->schedule(task);
}

void Kernel::schedule(PeriodicTask *periodicTask) {
  scheduler->schedule(periodicTask);
}

void Kernel::terminate(Task *task) {
  task->terminated();
  yield();
}

void Kernel::switchToTask(Task *task) {
  currentTask = task;
  currentTask->running();
  checkStackOverflow(currentTask);
  checkStackOverflow(scheduler);
  cpu->swapContext((uintptr_t *)&(scheduler->stack->pointer),
                   task->stack->pointer);
}

void Kernel::yield() {
  uintptr_t *stackPointerToStore = nullptr;
  if (currentTask != nullptr) {
    if (currentTask->isRunning()) {
      currentTask->ready();
    }
    stackPointerToStore = (uintptr_t *)&(currentTask->stack->pointer);
    checkStackOverflow(currentTask);
  }
  checkStackOverflow(scheduler);
  cpu->swapContext(stackPointerToStore, scheduler->stack->pointer);
}

void Kernel::preempt() {
  if (preemptionEnabled) {
    uintptr_t *stackPointerToStore = nullptr;
    if (currentTask != nullptr) {
      currentTask->ready();
      stackPointerToStore = (uintptr_t *)&currentTask->stack->pointer;
      checkStackOverflow(currentTask);
    }
    checkStackOverflow(scheduler);
    cpu->swapContext(stackPointerToStore, scheduler->stack->pointer);
  }
}
void Kernel::checkStackOverflow(Task *task) {
  if (!task->stack->isWithinBounds()) {
    Serial::send("\n\rStack Overflow\n\r");
  }
}

Promise *Kernel::await(Promise *promise) {
  if (!promise->isCompleted()) {
    currentTask->block();
    scheduler->add(currentTask, promise);
    auto stackPointerToStore = (uintptr_t *)&currentTask->stack->pointer;
    cpu->swapContext(stackPointerToStore,scheduler->stack->pointer);
  }

  return promise;
}

void Kernel::enablePreemption() {
  preemptionEnabled = true;
//  cpu->enableInterrupts();
}

void Kernel::disablePreemption() {
  preemptionEnabled = false;
//  cpu->disableInterrupts();
}

void Kernel::sleep(uint_fast16_t ms) {
  auto asyncSleep = await(new TimeWaitPromise(ms));
  delete asyncSleep;
}

void Kernel::incrementTick() {
  sysTicks->increment();
}

uint32_t Kernel::now() {
  return sysTicks->now();
}
bool Kernel::disableInterrupts() {
  if (interruptsEnabled) {
    interruptsEnabled = false;
    cpu->disableInterrupts();
    return true;
  }

  return false;
}
bool Kernel::enableInterrupts() {
  if (!interruptsEnabled) {
    interruptsEnabled = true;
    cpu->enableInterrupts();
    return true;
  }

  return false;
}
bool Kernel::isInterruptsEnabled() {
  return interruptsEnabled;
}
