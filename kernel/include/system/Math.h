//
// Created by ghisi on 1/3/23.
//

#ifndef AVR_MATH_H
#define AVR_MATH_H

#include "cstdint"

class Math {
public:
    template<typename T>
    static T min(T a, T b) {
            return a < b ? a : b;
    }

    template<typename T>
    static T max(T a, T b) {
        return a > b ? a : b;
    }

    static uint32_t divBy10(uint32_t n) {
        uint32_t q, r;
        q = (n >> 1) + (n >> 2);
        q = q + (q >> 4);
        q = q + (q >> 8);
        q = q + (q >> 16);
        q = q >> 3;
        r = n - (((q << 2) + q) << 1);
        return q + (r > 9);
    }
};


#endif //AVR_MATH_H
