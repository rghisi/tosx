//
// Created by ghisi on 11.03.24.
//

#ifndef TOSX_MUTEX_H
#define TOSX_MUTEX_H

#include <cstdint>

class Mutex {
public:
    bool acquire();
    void release();
private:
    uint32_t locked = 0;
};

#endif //TOSX_MUTEX_H
