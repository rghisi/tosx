//
// Created by ghisi on 11.09.22.
//

#ifndef AVR_ARRAY_H
#define AVR_ARRAY_H

#include "cstdint"

template <typename T>
class Array {
public:
    static void copy(T *source, uint8_t size, T *destination);
    static void copy(T *source, uint8_t size, T *destination, uint8_t offset);
    static void copy(T *source, uint8_t size, uint8_t offset, T *destination);
    static uint8_t *copy(T *source, uint8_t size);
    static bool isEquals(T *left, T *right, uint8_t size);
    static bool isEquals(T *left, uint8_t leftOffset, T *right, uint8_t size);
};

template<typename T>
inline void Array<T>::copy(T *source, uint8_t size, T *destination, uint8_t offset) {
    for (uint8_t i = 0; i < size; ++i) {
        destination[i + offset] = source[i];
    }
}

template<typename T>
inline void Array<T>::copy(T *source, uint8_t size, uint8_t offset, T *destination) {
    for (uint8_t i = 0; i < size; ++i) {
        destination[i] = source[i + offset];
    }
}

template<typename T>
inline void Array<T>::copy(T *source, uint8_t size, T *destination) {
    copy(source, size, destination, 0);
}

template<typename T>
bool Array<T>::isEquals(T *left, T *right, uint8_t size) {
    return isEquals(left, 0, right, size);
}

template<typename T>
bool Array<T>::isEquals(T *left, uint8_t leftOffset, T *right, uint8_t size) {
    uint8_t i = 0;
    bool isEqual = true;
    while (i < size && isEqual) {
        isEqual = left[i + leftOffset] == right[i];
        i++;
    }
    return isEqual;
}

template<typename T>
uint8_t *Array<T>::copy(T *source, uint8_t size) {
    auto *array = new T[size];
    copy(source, size, array);
    return array;
}


#endif //AVR_ARRAY_H
