//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_TIMEWAITPROMISE_H
#define AVR_TIMEWAITPROMISE_H

#include "Promise.h"
#include "cstdint"
#include "OS.h"

class TimeWaitPromise: public Promise {
public:
    explicit TimeWaitPromise(uint_fast16_t ms);
    bool isCompleted() override;
private:
    uint_fast32_t expiration;
};

TimeWaitPromise::TimeWaitPromise(uint16_t ms) {
    expiration = OS::now() + ms;
}

bool TimeWaitPromise::isCompleted() {
    return OS::now() >= expiration;
}

#endif //AVR_TIMEWAITPROMISE_H
