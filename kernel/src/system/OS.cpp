//
// Created by ghisi on 18.03.23.
//

#include <cstring>
#include <cstdio>
#include "system/OS.h"

#include "system/ExecutableTask.h"

Kernel* OS::kernel = nullptr;
MemoryAllocator *OS::memoryAllocator = nullptr;

void OS::schedule(Task *task) { kernel->schedule(task); }

void OS::schedule(PeriodicTask *task) { kernel->schedule(task); }

void OS::switchToTask(Task *task) { kernel->switchToTask(task); }

void OS::yield() { kernel->yield(); }

Promise *OS::await(Promise *promise) { return kernel->await(promise); }

void OS::sleep(uint_fast16_t ms) { kernel->sleep(ms); }

void OS::terminate(Task *task) { kernel->terminate(task); }

void *OS::memalloc(size_t len) {
  enterCritical();
  auto ptr = memoryAllocator->allocate(len);
  leaveCritical();
  if (ptr == nullptr) {
    printf("\n\rOut of Memory - Aborting\n\r");
    kernel->killCurrentTask();
  }
  return ptr;
}

void OS::memfree(void *p) {
  enterCritical();
  memoryAllocator->free(p);
  leaveCritical();
}

MemoryStats *OS::memoryStats() { return memoryAllocator->stats(); }

Task *OS::createTask(const char *name, int_fast8_t (*entryPoint)(char *), char *args) {
  auto argsCopy = new char[strlen(args) + 1];
  strcpy(argsCopy, args);
  auto *task = new ExecutableTask(name, entryPoint, argsCopy);
  return task;
}

void OS::preempt() { kernel->preempt(); }

void OS::incrementTick() { kernel->incrementTick(); }

uint32_t OS::now() { return kernel->now(); }

void OS::enterCritical() {
  if (kernel != nullptr) {
    kernel->enterCritical();
  }
}

void OS::leaveCritical() {
  if (kernel != nullptr) {
    kernel->leaveCritical();
  }
}
