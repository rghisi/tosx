//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_UPTIME_H
#define AVR_UPTIME_H

#include "cstdint"
#include "comms/Serial.h"
#include "system/OS.h"

class Uptime {
public:
    static int_fast8_t run(char* args) {
        auto totalSeconds = OS::now() / 1000;
        auto hours = (totalSeconds / 3600);
        auto minutes = (totalSeconds - (3600 * hours)) / 60;
        auto seconds = (totalSeconds - (3600 * hours) - (minutes * 60));
        Serial::send(static_cast<size_t>(hours));
        Serial::send(":");
        Serial::send(static_cast<size_t>(minutes));
        Serial::send(":");
        Serial::send(static_cast<size_t>(seconds));
        Serial::send("\n\r");
        return 0;
    }
};
#endif //AVR_UPTIME_H
