//
// Created by ghisi on 13.10.22.
//

#ifndef AVR_TASK_H
#define AVR_TASK_H

#include <cstdint>

#include "Stack.h"
#include "TaskState.h"
#include "TaskStats.h"
#include "StaticStack.h"

class Task {
public:
    explicit Task();
    virtual ~Task();
    virtual void execute() = 0;
    void initialize();
    TaskState state();
    virtual void running() final;
    virtual void ready() final;
    virtual void block() final;
    void terminated();
    bool isRunning();
    bool isCreated();
    bool isTerminated();
    bool isBlocked();
    const char *Name();
    uint_fast16_t Id();
    Stack *stack;
    static uint_fast16_t nextId;
protected:
    uint_fast16_t id;
    const char *name;
private:
    StaticStack<128> stackStack = StaticStack<128>();
    TaskState taskState = TaskState::CREATED;
};

#endif  // AVR_TASK_H
