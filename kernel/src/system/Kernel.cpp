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
  printf("\n\rStarting OS, setting up:\n\r");
  printf("\tWall clock\r\n");
  sysTicks = new SysTicks();
  cpu->setupSysTicks();
  printf("\tIdle task\n\r");
  idleTask = new IdleTask();
  cpu->initialize(idleTask);
  idleTask->ready();
  scheduler->setIdleTask(idleTask);
  printf("\tScheduler\n\r");
  cpu->initialize(scheduler);
  printf("\tPreemption\r\n");
  enablePreemption();
  printf("\tInterrupts\r\n");
  enableInterrupts();
  interruptsEnabled = true;
  reportMemory();
  printf("Switching to scheduler\r\n");
  startKernelTask();
  printf("\n\rPanic!\n\r");
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
  preemptionSuspended = true;
  currentTask = task;
  currentTask->running();
  checkStackOverflow(currentTask);
  checkStackOverflow(scheduler);
  runningTask = task;
  preemptionSuspended = false;
  cpu->swapContext((uintptr_t *)&(scheduler->stack->pointer),
                   task->stack->pointer);
}

void Kernel::yield() {
  if (runningTask != scheduler) {
    preemptionSuspended = true;
    uintptr_t *stackPointerToStore = nullptr;
    if (currentTask != nullptr) {
      if (currentTask->isRunning()) {
        currentTask->ready();
      }
      stackPointerToStore = (uintptr_t *)&(currentTask->stack->pointer);
      checkStackOverflow(currentTask);
    }
    checkStackOverflow(scheduler);
    runningTask = scheduler;
    preemptionSuspended = false;
    cpu->swapContext(stackPointerToStore, scheduler->stack->pointer);
  } else {
    printf("Yielding back to %s\r\n", currentTask->name);
    switchToTask(currentTask);
  }
}

void Kernel::preempt() {
  if (preemptionEnabled && !preemptionSuspended && runningTask != scheduler) {
    uintptr_t *stackPointerToStore = nullptr;
    if (currentTask != nullptr) {
      currentTask->ready();
      stackPointerToStore = (uintptr_t *)&currentTask->stack->pointer;
      checkStackOverflow(currentTask);
    }
    checkStackOverflow(scheduler);
    runningTask = scheduler;
    cpu->swapContext(stackPointerToStore, scheduler->stack->pointer);
  }
}
void Kernel::checkStackOverflow(Task *task) {
  if (!task->stack->isWithinBounds()) {
    printf("\n\rStack Overflow %s\n\r", task->name);
  }
}

Promise *Kernel::await(Promise *promise) {
  preemptionSuspended = true;
  if (!promise->isCompleted()) {
    currentTask->block();
    scheduler->add(currentTask, promise);
    auto stackPointerToStore = (uintptr_t *)&currentTask->stack->pointer;
    runningTask = scheduler;
    preemptionSuspended = false;
    cpu->swapContext(stackPointerToStore,scheduler->stack->pointer);
  }

  return promise;
}

void Kernel::enablePreemption() {
  preemptionEnabled = true;
}

void Kernel::disablePreemption() {
  preemptionEnabled = false;
}

void Kernel::sleep(uint_fast16_t ms) {
  auto promise = new TimeWaitPromise(ms);
  await(promise);
  delete promise;
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

void Kernel::reportMemory() {
  auto memoryStats = OS::memoryStats();
  auto total = memoryStats->used + memoryStats->free;
  auto free = memoryStats->free;
  printf("Total memory: %u, free %u\r\n", total, free);
}

void Kernel::enterCritical() {
  preemptionSuspended = true;
  while (criticalTask != nullptr) {
    printf("Concurrent Attempt by %s, blocked for %s\r\n", runningTask->name, criticalTask->name);
    criticalAttempt = true;
    yield();
  }
  criticalTask = runningTask;
  criticalAttempt = false;
}

void Kernel::leaveCritical() {
  if (runningTask != criticalTask) {
    printf("Critical left by %s instead of %s\r\n", runningTask->name, criticalTask->name);
  } else {
    criticalTask = nullptr;
    criticalAttempt = false;
  }
  preemptionSuspended = false;
}

void Kernel::killCurrentTask() {
  if (currentTask != nullptr) {
    printf("OOM Kill: %s", currentTask->name);
    terminate(currentTask);
  }
}
void Kernel::startKernelTask() {
  runningTask = scheduler;
  cpu->swapContext(nullptr, scheduler->stack->pointer);
}
