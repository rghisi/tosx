//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_MUTEX_H
#define AVR_MUTEX_H

#include "OS.h"
#include "../collections/LinkedList.h"

class Mutex {
public:
    void acquire();
    void release();
private:
    bool free = true;
    LinkedList<Promise*>* promises = new LinkedList<Promise*>();
};

void Mutex::acquire() {
    while (!free) {
        auto promise = new Promise();
        promises->push(promise);
        OS::await(promise);
    }
    free = false;
}

void Mutex::release() {
    free = true;
    promises->pop()->complete();
}


#endif //AVR_MUTEX_H
