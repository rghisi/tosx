//
// Created by ghisi on 19.04.24.
//

#ifndef TOSX_ARRAYLIST_H
#define TOSX_ARRAYLIST_H

#include <memory>
#include "FifoQueue.h"

template <typename T>
class ArrayList: public FifoQueue<T> {
public:
    ArrayList();
    ~ArrayList() override = default;
    ArrayList(const ArrayList& other);
    ArrayList& operator=(const ArrayList& other);
    ArrayList(ArrayList&& other) noexcept;
    ArrayList& operator=(ArrayList&& other) noexcept;

    size_t size() override;
    bool push(const T& item) override;
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
    std::unique_ptr<T[]> array;

    void growArray();
};

template<typename T>
ArrayList<T>::ArrayList() : array(std::make_unique<T[]>(INITIAL_CAPACITY)) {
}

template<typename T>
ArrayList<T>::ArrayList(const ArrayList &other) : capacity(other.capacity), elementCount(other.elementCount),
                                                  pushIndex(other.pushIndex), popIndex(other.popIndex),
                                                  array(std::make_unique<T[]>(other.capacity)) {
    for (size_t i = 0; i < elementCount; ++i) {
        array[i] = other.array[i];
    }
}

template<typename T>
void ArrayList<T>::growArray() {
    auto newCapacity = capacity << 1;
    auto newArray = std::make_unique<T[]>(newCapacity);
    size_t index = 0;
    for (size_t i = 0; i < elementCount; ++i) {
        newArray[i] = std::move(array[(popIndex + i) % capacity]);
    }
    capacity = newCapacity;
    popIndex = 0;
    pushIndex = elementCount;
    array = std::move(newArray);
}

template<typename T>
ArrayList<T>::ArrayList(ArrayList &&other) noexcept: capacity(other.capacity), elementCount(other.elementCount),
                                                     pushIndex(other.pushIndex), popIndex(other.popIndex),
                                                     array(std::move(other.array)) {
    other.capacity = 0;
    other.elementCount = 0;
    other.pushIndex = 0;
    other.popIndex = 0;
}

template<typename T>
ArrayList<T> &ArrayList<T>::operator=(const ArrayList &other) {
    if (this == &other) {
        return *this;
    }
    capacity = other.capacity;
    elementCount = other.elementCount;
    pushIndex = other.pushIndex;
    popIndex = other.popIndex;
    array = std::make_unique<T[]>(capacity);
    for (size_t i = 0; i < elementCount; ++i) {
        array[i] = other.array[i];
    }
    return *this;
}

template<typename T>
ArrayList<T> &ArrayList<T>::operator=(ArrayList &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    capacity = other.capacity;
    elementCount = other.elementCount;
    pushIndex = other.pushIndex;
    popIndex = other.popIndex;
    array = std::move(other.array);
    other.capacity = 0;
    other.elementCount = 0;
    other.pushIndex = 0;
    other.popIndex = 0;
    return *this;
}

template<typename T>
T ArrayList<T>::peek() {
    if (elementCount > 0) {
        return array[popIndex];
    }

    if constexpr (std::is_pointer<T>::value) {
        return nullptr;
    } else {
        return T{};
    }
}

template<typename T>
T ArrayList<T>::pop() {
    if (elementCount > 0) {
        elementCount--;
        T item = array[popIndex];
        popIndex = (popIndex + 1) % capacity;
        return item;
    }

    if constexpr (std::is_pointer<T>::value) {
        return nullptr;
    } else {
        return T{};
    }
}

template<typename T>
bool ArrayList<T>::push(const T& item) {
    if (isFull()) {
        growArray();
    }
    array[pushIndex] = item;
    pushIndex = (pushIndex + 1) % capacity;
    elementCount++;
    return true;
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
