//
// Created by ghisi on 26.10.22.
//

#include "collections/PriorityQueue.h"
#include "algorithm"

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
