//
// Created by ghisi on 19.03.23.
//

#ifndef AVR_STATICPRIORITYQUEUE_H
#define AVR_STATICPRIORITYQUEUE_H

#include <cstdint>
#include <cstddef>

template<typename T, size_t S>
class StaticPriorityQueue {
public:
    StaticPriorityQueue();
    bool offer(T *element);
    void pop();
    T* peek();
    bool isEmpty();
private:
    T* queue[S];
    size_t numberOfElements = 0;
    void pushElementsRightOf(size_t index);
    void pullElementsRightOf(size_t index);
};

template<typename T, size_t S>
StaticPriorityQueue<T, S>::StaticPriorityQueue() {
    for (size_t i = 0; i < S; i++) {
        queue[i] = nullptr;
    }
}

template<typename T, size_t S>
bool StaticPriorityQueue<T, S>::offer(T *element) {
    if (numberOfElements == S) {
        return false;
    }

    size_t insertionIndex;
    T *valueAtInsertionIndex = nullptr;
    for (insertionIndex = 0; insertionIndex < S; insertionIndex++) {
        valueAtInsertionIndex = queue[insertionIndex];
        if (valueAtInsertionIndex == nullptr || *element <= *valueAtInsertionIndex) {
            break;
        }
    }

    if (valueAtInsertionIndex != nullptr) {
        pushElementsRightOf(insertionIndex);
    }

    queue[insertionIndex] = element;
    numberOfElements++;

    return true;
}

template<typename T, size_t S>
bool StaticPriorityQueue<T, S>::isEmpty() {
    return numberOfElements == 0;
}

template<typename T, size_t S>
T *StaticPriorityQueue<T, S>::peek() {
    if (numberOfElements == 0) {
        return nullptr;
    }

    return queue[0];
}

template<typename T, size_t S>
void StaticPriorityQueue<T, S>::pop() {
    if (numberOfElements > 0) {
        pullElementsRightOf(0);
        numberOfElements--;
    }
}

template<typename T, size_t S>
void StaticPriorityQueue<T, S>::pushElementsRightOf(size_t index) {
    for (size_t i = (S - 2); i > index; i--) {
        queue[i] = queue[i - 1];
    }
}

template<typename T, size_t S>
void StaticPriorityQueue<T, S>::pullElementsRightOf(size_t index) {
    for (size_t i = index; i < (S - 1); i++) {
        queue[i] = queue[i+1];
    }
    queue[S - 1] = nullptr;
}

#endif //AVR_STATICPRIORITYQUEUE_H
