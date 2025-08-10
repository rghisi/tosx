#include "system/Mutex.h"

bool Mutex::acquire() {
    if (locked) {
        return false;
    }
    locked = 1;
    return true;
}

void Mutex::release() {
    locked = 0;
}
