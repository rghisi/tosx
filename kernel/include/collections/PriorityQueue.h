//
// Created by ghisi on 26.10.22.
//

#ifndef AVR_PRIORITYQUEUE_H
#define AVR_PRIORITYQUEUE_H

#include <cstdint>
#include "memory"
#include "list"

template <typename T>
class PriorityQueue {
public:
    bool offer(T element);
    void pop();
    T peek();
    bool isEmpty();
private:
    std::list<T> list;
};


#endif //AVR_PRIORITYQUEUE_H
