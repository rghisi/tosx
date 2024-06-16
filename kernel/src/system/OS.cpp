//
// Created by ghisi on 18.03.23.
//

#include <cstring>
#include <cstdio>
#include "system/OS.h"

Kernel* OS::kernel = nullptr;

void OS::schedule(Task *task) { kernel->schedule(task); }

void OS::schedule(PeriodicTask *task) { kernel->schedule(task); }

void OS::switchToTask(Task *task) { kernel->switchToTask(task); }

void OS::yield() { kernel->switchToScheduler(); }

Promise *OS::await(Promise *promise) { return kernel->await(promise); }

void OS::sleep(uint_fast16_t ms) { kernel->sleep(ms); }

void OS::terminate(Task *task) { kernel->terminate(task); }

void *OS::memalloc(size_t len) {
  return kernel->allocateMemory(len);
}

void OS::memfree(void *p) {
 kernel->freeMemory(p);
}

MemoryStats *OS::memoryStats() {
    return kernel->GetMemoryStats();
}

Task *OS::createTask(const char *name, int_fast8_t (*entryPoint)(char *), char *args) {
  return kernel->CreateTask(name, entryPoint, args);
}

void OS::preempt() { kernel->preempt(); }

void OS::incrementTick() { kernel->incrementTick(); }

uint32_t OS::now() { return kernel->now(); }

void OS::PrintProcesses() {
    kernel->PrintProcesses();
}

void OS::MemoryDump() {
    kernel->MemoryDump();
}
