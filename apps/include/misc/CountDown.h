//
// Created by ghisi on 08.03.24.
//

#ifndef AVR_COUNTDOWN_H
#define AVR_COUNTDOWN_H

#include "cstdint"
#include "comms/Serial.h"
#include "shell/CommandLine.h"
//#include <util/delay.h>


class CountDown {
public:
    static int_fast8_t run(char* args);
private:
    static constexpr const char* NEW_LINE = "\n\r";
};

int_fast8_t CountDown::run(char *args) {
    auto commandLine = new CommandLine(args);
    auto instance = commandLine->parameter(0)[0];
    delete commandLine;
    for (uint8_t c = 10; c > 0; c--) {
        Serial::send(instance);
        Serial::send((size_t)c);
        Serial::send(NEW_LINE);
        for (uintptr_t t = 0; t < 8000000; t++){
          __asm volatile ("nop");
        }
    }
    Serial::send("End of ");
    Serial::send(instance);
    Serial::send(NEW_LINE);
    return 0;
}

#endif //AVR_COUNTDOWN_H
