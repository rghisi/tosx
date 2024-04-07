//
// Created by ghisi on 18.03.23.
//


#include "cstring"
#include "cstdio"
#include "system/OS.h"
#include "system/ExecutableTask.h"
#include "comms/Serial.h"

void OS::schedule(Task *task) {
    kernel->schedule(task);
}

void OS::schedule(PeriodicTask *task) {
    kernel->schedule(task);
}

void OS::switchToTask(Task *task) {
    kernel->switchToTask(task);
}

void OS::yield() {
    kernel->yield();
}

Promise* OS::await(Promise *promise) {
    return kernel->await(promise);
}

void OS::sleep(uint_fast16_t ms) {
    kernel->sleep(ms);
}

void *OS::memalloc(size_t len) {
    disablePreemption();
    auto ptr = memoryAllocator->allocate(len);
    if (ptr == nullptr) {
        Serial::send("\n\rOut of Memory - Aborting\n\r");
//        OS::cpu->disableInterrupts();
        while (true);
    }
    enablePreemption();
    return ptr;
}

void OS::memfree(void *p) {
    disablePreemption();
    memoryAllocator->free(p);
    enablePreemption();
}

MemoryStats *OS::memoryStats() {
    return memoryAllocator->stats();
}

Task *OS::createTask(int_fast8_t (*entryPoint)(char *), char *args) {
    //switch context or allocator, then...
    auto* task = new ExecutableTask(entryPoint, args);
    return task;
}

void OS::preempt() {
    kernel->preempt();
}

void OS::enablePreemption() {
    kernel->enablePreemption();
}

void OS::disablePreemption() {
    kernel->disablePreemption();
}

void OS::incrementTick() {
    kernel->incrementTick();
}

uint32_t OS::now() {
    return kernel->now();
}
