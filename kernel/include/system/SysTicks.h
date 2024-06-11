//
// Created by ghisi on 17.09.22.
//

#ifndef AVR_SYSTICKS_H
#define AVR_SYSTICKS_H

#include "cstdint"

class SysTicks {
public:
    void increment();
    [[nodiscard]] uint32_t now() const;
private:
    uint32_t ticks = 0;
};

#endif //AVR_SYSTICKS_H
