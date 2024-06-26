//
// Created by ghisi on 08.03.24.
//

#ifndef AVR_COUNTDOWN_H
#define AVR_COUNTDOWN_H

#include "comms/Serial.h"
#include "cstdint"
#include "cstdio"
#include "shell/CommandLine.h"
// #include <util/delay.h>


class CountDown {
public:
    static int_fast8_t run(char* args);
};

int_fast8_t CountDown::run(char *args) {
    auto commandLine = CommandLine(args);
    auto instance = commandLine.parameter(0).at(0);
    for (uint8_t counter = 10; counter > 0; counter--) {
        printf("%c:%u\r\n", instance, counter);
        for (uint32_t t = 0; t < 8000000; t++){
          __asm volatile ("nop");
        }
    }
    printf("End of %c\r\n", instance);
    return 0;
}

#endif //AVR_COUNTDOWN_H
