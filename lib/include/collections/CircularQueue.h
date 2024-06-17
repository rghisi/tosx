//
// Created by ghisi on 31.08.22.
//

#ifndef AVR_CIRCULARQUEUE_H
#define AVR_CIRCULARQUEUE_H
#include <algorithm>
#include <cstdint>
#include <cstddef>
#include <span>
#include "type_traits"
#include "FifoQueue.h"

template <typename T, size_t S>
class CircularQueue: public FifoQueue<T>{
public:
    CircularQueue();
    bool push(T item) override;
    T pop() override;
    T peek() override;
    bool isEmpty() override;
    bool isFull() override;
    size_t size() override;
    void clear();

private:
    static constexpr const uint_fast8_t CAPACITY = S;
    uint_fast8_t used;
    uint_fast8_t pushIndex;
    uint_fast8_t popIndex;
    T queue[S];
};

template <typename T, std::size_t S>
CircularQueue<T, S>::CircularQueue() {
    pushIndex = 0;
    popIndex = 0;
    used = 0;
}

template <typename T, std::size_t S>
bool CircularQueue<T, S>::isEmpty() {
    return used == 0;
}

template <typename T, std::size_t S>
bool CircularQueue<T, S>::isFull() {
    return used == CAPACITY;
}

template<typename T, std::size_t S>
bool CircularQueue<T, S>::push(T item) {
    if (used < CAPACITY) {
        used++;
        queue[pushIndex] = item;
        pushIndex++;
        if (pushIndex == CAPACITY) {
            pushIndex = 0;
        }

        return true;
    }

    return false;
}

template<typename T, std::size_t S>
T CircularQueue<T, S>::pop() {
    if (used > 0) {
        used--;
        T item = this->queue[popIndex];
        if constexpr (std::is_integral<T>::value) {
            this->queue[popIndex] = 0;
        } else {
            this->queue[popIndex] = nullptr;
        }
        popIndex++;
        if (popIndex == CAPACITY) {
            popIndex = 0;
        }
        return item;
    }

    if constexpr (std::is_pointer<T>::value) {
        return nullptr;
    } else {
        return 0;
    }
}

template<typename T, size_t S>
T CircularQueue<T, S>::peek() {
    if (used > 0) {
        return this->queue[popIndex];
    }

    if constexpr (std::is_pointer<T>::value) {
        return nullptr;
    } else {
        return 0;
    }
}

template<typename T, std::size_t S>
void CircularQueue<T, S>::clear() {
    this->pushIndex = 0;
    this->popIndex = 0;
    this->used = 0;
}

template<typename T, std::size_t S>
size_t CircularQueue<T, S>::size() {
    return used;
}

#endif //AVR_CIRCULARQUEUE_H
