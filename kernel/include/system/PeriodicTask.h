//
// Created by ghisi on 12.11.23.
//

#ifndef AVR_PERIODICTASK_H
#define AVR_PERIODICTASK_H

#include "Task.h"
#include "StaticStack.h"

class PeriodicTask: public Task {
public:
    explicit PeriodicTask(): Task() {
        stack = std::make_unique<StaticStack<256>>();
    }
    virtual uint_fast16_t period() = 0;
};

#endif //AVR_PERIODICTASK_H
