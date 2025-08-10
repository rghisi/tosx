//
// Created by ghisi on 09.03.24.
//

#ifndef AVR_EXECUTABLETASK_H
#define AVR_EXECUTABLETASK_H


#include "Task.h"
#include "TaskState.h"
#include "DynamicStack.h"

class ExecutableTask: public Task {
public:
    ExecutableTask(const char* name, int_fast8_t (*entryPoint)(char *), char *args, size_t stackSize = 512);
    ~ExecutableTask() override;
    void execute() override;
private:
    int_fast8_t (*entryPointFunction)(char *);
    char *args;
};

ExecutableTask::ExecutableTask(const char* name, int_fast8_t (*entryPoint)(char *), char *args, size_t stackSize) {
    this->entryPointFunction = entryPoint;
    this->args = args;
    this->name = name;
    this->stack = std::make_unique<DynamicStack>(stackSize);
}

void ExecutableTask::execute() {
    entryPointFunction(args);
}

ExecutableTask::~ExecutableTask() {
    delete[] args;
}


#endif //AVR_EXECUTABLETASK_H
