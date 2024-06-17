//
// Created by ghisi on 4/28/24.
//

#ifndef TOSX_PS_H
#define TOSX_PS_H

#include <cstdint>
#include "system/OS.h"

class Ps {
public:
    static int_fast8_t run(char *args);
};

int_fast8_t Ps::run(char *args) {
    OS::PrintProcesses();
    return 0;
}

#endif  // TOSX_PS_H
