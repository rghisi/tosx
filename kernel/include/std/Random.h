//
// Created by ghisi on 10.09.22.
//

#ifndef AVR_RANDOM_H
#define AVR_RANDOM_H

#include <stdint.h>

class Random {
public:
    static uint8_t next();
    static void seed(uint8_t);
private:
    static uint8_t _next(uint8_t previous);
    static uint8_t last;
    static uint8_t offset;
};


#endif //AVR_RANDOM_H
