//
// Created by ghisi on 13.10.22.
//

#ifndef AVR_TASK_H
#define AVR_TASK_H

#include <cstdint>
#include "Stack.h"
#include "TaskState.h"
#include "TaskStats.h"

class Task {
public:
    explicit Task();
    virtual ~Task();
    virtual void execute() = 0;
    void initialize();
    TaskState state();
    void running();
    void waiting();
    void block();
    Stack *stack;
//    TaskStats stats();
protected:
    TaskState taskState = TaskState::CREATED;
//    TaskStats taskStats = TaskStats();
//    MemoryAllocator<128> *memoryAllocator;
};

#endif //AVR_TASK_H
