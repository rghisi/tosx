//
// Created by ghisi on 12.11.23.
//

#ifndef AVR_STACK_H
#define AVR_STACK_H


#include "cstdint"

class Stack {
public:
    virtual ~Stack() = default;
    virtual bool isWithinBounds() = 0;
    volatile uintptr_t pointer = 0;
    volatile uintptr_t start = 0;
    volatile uintptr_t end = 0;
};

#endif //AVR_STACK_H
