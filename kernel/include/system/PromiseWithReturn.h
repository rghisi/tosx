//
// Created by ghisi on 12.11.23.
//

#ifndef AVR_PROMISEWITHRETURN_H
#define AVR_PROMISEWITHRETURN_H


#include "Promise.h"
#include "type_traits"
#include <cstddef>

template <typename T>
class PromiseWithReturn: public Promise {
public:
    ~PromiseWithReturn() override {
        if constexpr (std::is_pointer<T>::value)
            delete data;
    }
    T data;
    size_t size = 0;
};
#endif //AVR_PROMISEWITHRETURN_H
