//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_LINKEDLIST_H
#define AVR_LINKEDLIST_H

#include "FifoQueue.h"

template <typename T>
class LinkedList: public FifoQueue<T> {
public:
    bool push(T t) override;
    T pop() override;
    T peek() override;
    size_t size() override;
    bool isEmpty() override;
    bool isFull() override;
private:
    template <typename V>
    class Node {
    public:
        explicit Node(V v) {
            value = v;
            next = nullptr;
        }
        V value;
        Node* next;
    };

    Node<T>* first = nullptr;
    Node<T>* last = nullptr;
    size_t numberOfElements = 0;
};

template<typename T>
bool LinkedList<T>::isFull() {
    return false;
}

template<typename T>
size_t LinkedList<T>::size() {
    return numberOfElements;
}

template<typename T>
bool LinkedList<T>::isEmpty() {
    return first == nullptr;
}

template<typename T>
bool LinkedList<T>::push(T t) {
    auto node = new Node<T>(t);
    if (first == nullptr) {
        first = node;
        last = first;
    } else {
        last->next = node;
        last = node;
    }
    numberOfElements++;

    return true;
}

template<typename T>
T LinkedList<T>::pop() {
    auto value = first->value;
    if (first == last) {
        delete first;
        first = nullptr;
        last = nullptr;
    } else {
        auto popped = first;
        first = first->next;
        delete popped;
    }
    numberOfElements--;

    return value;
}

template<typename T>
T LinkedList<T>::peek() {
    return first->value;
}


#endif //AVR_LINKEDLIST_H
