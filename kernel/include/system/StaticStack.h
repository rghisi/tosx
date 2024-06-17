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
        pointer = &stack[S - 1];
        start = &(stack[S]);
        end = &(stack[0]);
    }
    ~StaticStack() override = default;
    bool isWithinBounds() override;
    size_t Used() override;
    size_t Free() override;
private:
    uintptr_t stack[S] = {};
};

template<size_t S>
bool StaticStack<S>::isWithinBounds() {
    return pointer < start && pointer > end;
}

template<size_t S>
size_t StaticStack<S>::Used() {
    return start - pointer;
}

template<size_t S>
size_t StaticStack<S>::Free() {
    return pointer - end;
}

#endif //AVR_STATICSTACK_H
