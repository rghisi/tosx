//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_KEYPRESSEVENT_H
#define AVR_KEYPRESSEVENT_H

#include "cstdint"
#include "../system/Event.h"

class KeyPressEvent: public Event {
public:
    explicit KeyPressEvent(char key) {
        this->key = key;
    }
    char key;
    const uint_fast16_t eventType = 123;
};


#endif //AVR_KEYPRESSEVENT_H
