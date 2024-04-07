//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_CPU_H
#define AVR_CPU_H

#include "Task.h"

class Cpu {
public:
    virtual void enableInterrupts() = 0;
    virtual void disableInterrupts() = 0;
    virtual void setupSysTicks() = 0;
    virtual void startTask(Task *task) = 0;
    virtual void switchToTask(Task *task) = 0;
    virtual void startKernelTask(Task *kernelTask) = 0;
    virtual void switchToKernel(Task* task) = 0;
};


#endif //AVR_CPU_H
