//
// Created by ghisi on 21.03.24.
//

#include "system/Kernel.h"

#include <cstdio>
#include <cstring>

#include "comms/Serial.h"
#include "system/TaskState.h"
#include "system/TimeWaitPromise.h"
#include "system/ExecutableTask.h"

Kernel::Kernel(Cpu *cpu, Scheduler *scheduler, MemoryAllocator *memoryAllocator) {
    this->cpu = cpu;
    this->scheduler = scheduler;
    this->memoryAllocator = memoryAllocator;
}

void Kernel::start() {
    printf("Starting Kernel\n\r");
    printf("\tWall clock\r\n");
    sysTicks = new SysTicks();
    cpu->setupSysTicks();
    printf("\tIdle task\n\r");
    idleTask = new IdleTask();
    cpu->initialize(idleTask);
    idleTask->ready();
    tasks.push_back(idleTask);
    scheduler->setIdleTask(idleTask);
    printf("\tScheduler\n\r");
    cpu->initialize(scheduler);
    scheduler->ready();
    tasks.push_back(scheduler);
    reportMemory();
    printf("\tPreemption\r\n");
    enablePreemption();
    printf("\tInterrupts\r\n");
    enableInterrupts();
    interruptsEnabled = true;
    printf("Switching to scheduler\r\n");
    startKernelTask();
    printf("\n\rPanic!\n\r");
}

void Kernel::schedule(Task *task) {
    if (task->isCreated()) {
        cpu->initialize(task);
        task->ready();
    }
    tasks.push_back(task);
    scheduler->schedule(task);
}

void Kernel::schedule(PeriodicTask *periodicTask) {
    scheduler->schedule(periodicTask);
}

void Kernel::terminate(Task *task) {
    task->terminated();
    tasks.remove(task);
    switchToScheduler();
}

void Kernel::switchToTask(Task *task) {
    preemptionSuspended = true;
    currentTask = task;
    currentTask->running();
    checkStackOverflow(currentTask);
    checkStackOverflow(scheduler);
    runningTask = task;
    runningTaskId = runningTask->Id();
    preemptionSuspended = false;
    cpu->swapContext((uintptr_t *) &(scheduler->stack->pointer), task->stack->pointer);
}

void Kernel::switchToScheduler() {
    if (runningTask != scheduler && memoryAllocationMutex.acquire()) {
        preemptionSuspended = true;
        uintptr_t *stackPointerToStore = nullptr;
        if (currentTask != nullptr) {
            if (currentTask->isRunning()) {
                currentTask->ready();
            }
            stackPointerToStore = (uintptr_t *) &(currentTask->stack->pointer);
            checkStackOverflow(currentTask);
        }
        checkStackOverflow(scheduler);
        runningTask = scheduler;
        runningTaskId = runningTask->Id();
        memoryAllocationMutex.release();
        preemptionSuspended = false;
        cpu->swapContext(stackPointerToStore, scheduler->stack->pointer);
    }
}

void Kernel::preempt() {
    if (preemptionEnabled && !preemptionSuspended) {
        switchToScheduler();
    }
}

void Kernel::checkStackOverflow(Task *task) {
    if (!task->stack->isWithinBounds()) {
        printf("\n\rStack Overflow %s\n\r", task->Name());
    }
}

Promise *Kernel::await(Promise *promise) {
    preemptionSuspended = true;
    if (!promise->isCompleted()) {
        currentTask->block();
        scheduler->add(currentTask, promise);
        auto stackPointerToStore = (uintptr_t *) &currentTask->stack->pointer;
        runningTask = scheduler;
        runningTaskId = runningTask->Id();
        preemptionSuspended = false;
        cpu->swapContext(stackPointerToStore, scheduler->stack->pointer);
    }

    return promise;
}

void Kernel::enablePreemption() {
    preemptionEnabled = true;
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

void Kernel::startKernelTask() {
    runningTask = scheduler;
    runningTaskId = runningTask->Id();
    cpu->swapContext(nullptr, scheduler->stack->pointer);
}

uint_fast16_t Kernel::RunningTaskId() {
    return runningTaskId;
}

std::list<Task *> Kernel::Tasks() {
    return tasks;
}

Task *Kernel::CreateTask(const char *name, int_fast8_t (*entryPoint)(char *), char *args) {
    auto previouslyRunningTaskId = runningTaskId;
    runningTaskId = Task::nextId;
    auto argsCopy = new char[strlen(args) + 1];
    strcpy(argsCopy, args);
    auto *task = new ExecutableTask(name, entryPoint, argsCopy);
    runningTaskId = previouslyRunningTaskId;
    return task;
}

void *Kernel::allocateMemory(size_t size) {
    while (!memoryAllocationMutex.acquire()) {
        if (runningTask == scheduler) {
            Serial::send('Z');
            switchToTask(currentTask);
        } else {
            Serial::send('z');
            switchToScheduler();
        }
    }
    auto ptr = memoryAllocator->allocate(size, RunningTaskId());
    memoryAllocationMutex.release();
    if (ptr == nullptr) {
        printf("\n\rOut of Memory - cannot allocate %u bytes - only %u bytes free - Aborting\n\r",
               size,
               memoryAllocator->stats()->free
        );
        if (currentTask != nullptr) {
            printf("OOM Kill: %s (%d)\r\n", currentTask->Name(), currentTask->Id());
            terminate(currentTask);
        }
    }

    return ptr;
}

void Kernel::freeMemory(void *allocation) {
    while (!memoryAllocationMutex.acquire()) {
        if (runningTask == scheduler) {
            Serial::send('X');
            switchToTask(currentTask);
        } else {
            Serial::send('x');
            switchToScheduler();
        }
    }
    memoryAllocator->free(allocation);
    memoryAllocationMutex.release();
}

MemoryStats *Kernel::GetMemoryStats() {
    return memoryAllocator->stats();
}

void Kernel::PrintProcesses() {
    printf("ID\tProcess\t\t\tStatus\tHeap(u)\tStack(u)/(f)\r\n");
    for (auto task: tasks) {
        auto memory = memoryAllocator->UsedMemory(task->Id());
        char state;
        switch (task->state()) {
            case TaskState::CREATED:
                state = 'C';
                break;
            case TaskState::READY:
                state = 'W';
                break;
            case TaskState::RUNNING:
                state = 'R';
                break;
            case TaskState::BLOCKED:
                state = 'B';
                break;
            case TaskState::TERMINATED:
                state = 'T';
                break;
        }
        printf("%d\t%-20s\t%c\t%d\t%d/%d\r\n",
               task->Id(),
               task->Name(),
               state,
               memory,
               task->stack->Used(),
               task->stack->Free());
    }
}

void Kernel::MemoryDump() {
    memoryAllocator->Dump();
}
