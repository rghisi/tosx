//
// Created by ghisi on 21.03.24.
//

#include "system/Kernel.h"
#include "comms/Serial.h"
#include "system/TimeWaitPromise.h"
#include "system/TaskState.h"

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
    Serial::send("OK\n\r");
    Serial::send("\tEnabling Preemption: ");
    enablePreemption();
    Serial::send("OK\n\r");
    Serial::send("\tStarting Kernel Task\n\r");
    cpu->startKernelTask(scheduler);
    Serial::send("\n\rPanic! Kernel Task Ended\n\r");
    while (true);
}

void Kernel::schedule(Task *task) {
    scheduler->schedule(task);
}

void Kernel::schedule(PeriodicTask *periodicTask) {
    scheduler->schedule(periodicTask);
}

void Kernel::switchToTask(Task *task) {
    currentTask = task;
    if (currentTask->state() == TaskState::CREATED) {
        currentTask->running();
        cpu->startTask(task);
    } else {
        currentTask->running();
        cpu->switchToTask(task);
    }
}

void Kernel::yield() {
    if (currentTask != nullptr) {
        currentTask->waiting();
        if (!currentTask->stack->isWithinBounds()) {
            Serial::send("\n\rStack Overflow\n\r");
        }
        cpu->switchToKernel(currentTask);
    }
}

void Kernel::preempt() {
    if (preemptionEnabled) {
        if (currentTask != nullptr) {
            currentTask->waiting();
            if (!currentTask->stack->isWithinBounds()) {
                Serial::send("\n\rStack Overflow\n\r");
            }
        }
        cpu->switchToKernel(currentTask);
    }
}

void Kernel::enablePreemption() {
    preemptionEnabled = true;
}

void Kernel::disablePreemption() {
    preemptionEnabled = false;
}

Promise *Kernel::await(Promise *promise) {
    if (!promise->isCompleted()) {
        currentTask->block();
        scheduler->add(currentTask, promise);
        cpu->switchToKernel(currentTask);
    }

    return promise;
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
