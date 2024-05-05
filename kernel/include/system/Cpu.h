//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_CPU_H
#define AVR_CPU_H

#include "Task.h"

class Cpu {
public:
    virtual void setup() = 0;
    virtual void enableInterrupts() = 0;
    virtual void disableInterrupts() = 0;
    virtual void setupSysTicks() = 0;
    virtual void initialize(Task*) = 0;
    virtual void swapContext(uintptr_t *stackPointerToStore, uintptr_t *stackPointerToLoad) = 0;
};


#endif //AVR_CPU_H
