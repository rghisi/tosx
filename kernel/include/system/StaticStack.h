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
        stack[0] = 0xAFADAFAD;
    }
    ~StaticStack() override = default;
    bool isWithinBounds() override;
private:
    uintptr_t stack[S] = {};
};

//template<size_t S>
//StaticStack<S>::~StaticStack() {
////    delete stack;
//}

template<size_t S>
bool StaticStack<S>::isWithinBounds() {
    return pointer < start && pointer > end && stack[0] == 0xAFADAFAD;
}

#endif //AVR_STATICSTACK_H
