//
// Created by ghisi on 12.11.23.
//

#ifndef AVR_STATICSTACK_H
#define AVR_STATICSTACK_H

#include "cstddef"
#include "Stack.h"

template <size_t S>
class StaticStack: public Stack {
public:
    StaticStack() {
        stack = new uintptr_t[S];
        pointer = reinterpret_cast<volatile uintptr_t>(&stack[S - 1]);
        start = reinterpret_cast<volatile uintptr_t>(&(stack[S]));
        end = reinterpret_cast<volatile uintptr_t>(&(stack[0]));
    }
    ~StaticStack() override;
    bool isWithinBounds() override;
private:
    volatile uintptr_t *stack;
};

template<size_t S>
StaticStack<S>::~StaticStack() {
    delete[] stack;
}

template<size_t S>
bool StaticStack<S>::isWithinBounds() {
    return pointer < start && pointer > end;
}

#endif //AVR_STATICSTACK_H
