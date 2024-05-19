//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_CLEAR_H
#define AVR_CLEAR_H

#include "cstdint"
#include "comms/Serial.h"

class Clear {
public:
    static int_fast8_t run(char* args) {
        Serial::send(CLEAR_SCREEN);
        return 0;
    }

private:
    static constexpr const char* CLEAR_SCREEN = "\x1b[2J\x1b[H";
};
#endif //AVR_CLEAR_H
