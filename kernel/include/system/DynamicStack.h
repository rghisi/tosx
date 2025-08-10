#ifndef AVR_DYNAMICSTACK_H
#define AVR_DYNAMICSTACK_H

#include "Stack.h"
#include "system/OS.h"
#include <cstddef>

class DynamicStack : public Stack {
public:
    explicit DynamicStack(size_t size) {
        this->size = size;
        this->buffer = (uintptr_t*)OS::memalloc(size);
        this->pointer = &buffer[size / sizeof(uintptr_t) - 1];
        this->start = &buffer[size / sizeof(uintptr_t)];
        this->end = &buffer[0];
    }

    ~DynamicStack() override {
        OS::memfree(buffer);
    }

    bool isWithinBounds() override {
        return pointer < start && pointer > end;
    }

    size_t Used() override {
        return (start - pointer) * sizeof(uintptr_t);
    }

    size_t Free() override {
        return (pointer - end) * sizeof(uintptr_t);
    }

private:
    size_t size;
    uintptr_t* buffer;
};

#endif //AVR_DYNAMICSTACK_H
