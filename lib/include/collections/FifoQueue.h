//
// Created by ghisi on 26.03.24.
//

#ifndef AVR_FIFOQUEUE_H
#define AVR_FIFOQUEUE_H


#include "cstddef"

template <typename T>
class FifoQueue {
public:
    virtual ~FifoQueue() = default;
    virtual size_t size() = 0;
    virtual bool push(const T&) = 0;
    virtual T pop() = 0;
    virtual T peek() = 0;
    virtual bool isEmpty() = 0;
    virtual bool isFull() = 0;
};


#endif //AVR_FIFOQUEUE_H
