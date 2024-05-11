//
// Created by ghisi on 18.03.23.
//

#include "system/OS.h"

#include "comms/Serial.h"
#include "system/ExecutableTask.h"

Kernel* OS::kernel = nullptr;

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
    Serial::send("\n\rOut of Memory - Aborting\n\r");
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

Task *OS::createTask(const char *name, int_fast8_t (*entryPoint)(char *),
                     char *args) {
  auto *task = new ExecutableTask(name, entryPoint, args);
  return task;
}

void OS::preempt() { kernel->preempt(); }

void OS::enablePreemption() { kernel->enablePreemption(); }

void OS::disablePreemption() { kernel->disablePreemption(); }

void OS::incrementTick() { kernel->incrementTick(); }

uint32_t OS::now() { return kernel->now(); }

bool OS::disableInterupts() {
  if (kernel == nullptr) {
    return false;
  }
  return kernel->disableInterrupts();
}

bool OS::enableInterupts() {
  if (kernel == nullptr) {
    return false;
  }
  return kernel->enableInterrupts();
}

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
