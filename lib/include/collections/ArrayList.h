//
// Created by ghisi on 19.04.24.
//

#ifndef TOSX_ARRAYLIST_H
#define TOSX_ARRAYLIST_H

#include "FifoQueue.h"

template <typename T>
class ArrayList: public FifoQueue<T> {
public:
    size_t size() override;
    bool push(T item) override;
    T pop() override;
    T peek() override;
    bool isEmpty() override;
    bool isFull() override;
private:
    static constexpr size_t INITIAL_CAPACITY = 2;
    size_t capacity = INITIAL_CAPACITY;
    size_t elementCount = 0;
    uint_fast8_t pushIndex = 0;
    uint_fast8_t popIndex = 0;
    T *array = new T[INITIAL_CAPACITY];

    void growArray();
};

template<typename T>
void ArrayList<T>::growArray() {
    auto newCapacity = capacity << 1;
    auto newArray = new T[newCapacity];
    size_t index = 0;
    while (!this->isEmpty()) {
        newArray[index] = this->pop();
        index++;
    }
    capacity = newCapacity;
    popIndex = 0;
    pushIndex = index;
    elementCount = index;
    delete[] array;
    array = newArray;
}

template<typename T>
T ArrayList<T>::peek() {
    if (elementCount > 0) {
        return this->array[popIndex];
    }

    if constexpr (std::is_pointer<T>::value) {
        return nullptr;
    } else {
        return 0;
    }
}

template<typename T>
T ArrayList<T>::pop() {
    if (elementCount > 0) {
        elementCount--;
        T item = this->array[popIndex];
        if constexpr (std::is_integral<T>::value) {
            this->array[popIndex] = 0;
        } else {
            this->array[popIndex] = nullptr;
        }
        popIndex++;
        if (popIndex == capacity) {
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

template<typename T>
bool ArrayList<T>::push(T item) {
    if (elementCount < capacity) {
        elementCount++;
        array[pushIndex] = item;
        pushIndex++;
        if (pushIndex == capacity) {
            pushIndex = 0;
        }

        return true;
    } else {
        growArray();
        return push(item);
    }
}

template<typename T>
bool ArrayList<T>::isFull() {
    return elementCount == capacity;
}

template<typename T>
bool ArrayList<T>::isEmpty() {
    return elementCount == 0;
}

template<typename T>
size_t ArrayList<T>::size() {
    return elementCount;
}


#endif //TOSX_ARRAYLIST_H
