//
// Created by ghisi on 21.03.24.
//

#include "system/SysTicks.h"

void SysTicks::increment() {
    ticks++;
}

uint32_t SysTicks::now() const {
    return ticks;
}