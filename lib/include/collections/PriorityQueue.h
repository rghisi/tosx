//
// Created by ghisi on 26.10.22.
//

#ifndef AVR_PRIORITYQUEUE_H
#define AVR_PRIORITYQUEUE_H

#include <cstdint>
#include "memory"
#include "list"
#include "algorithm"

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

template<typename T>
bool PriorityQueue<T>::offer(T element) {
    auto found = std::find_if(list.begin(), list.end(), [&element](T e){ return *element <= *e;});
    list.insert(found, element);

    return true;
}

template<typename T>
void PriorityQueue<T>::pop() {
    if (!list.empty()) {
        list.pop_front();
    }
}

template<typename T>
bool PriorityQueue<T>::isEmpty() {
    return list.empty();
}

template<typename T>
T PriorityQueue<T>::peek() {
    return list.front();
}

#endif //AVR_PRIORITYQUEUE_H
