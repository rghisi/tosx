//
// Created by ghisi on 08.03.24.
//

#ifndef AVR_ECHO_H
#define AVR_ECHO_H

#include "cstdint"
#include "comms/Serial.h"
#include "shell/CommandLine.h"


class Echo {
public:
    static int_fast8_t run(char* args);
};

int_fast8_t Echo::run(char *args) {
    auto commandLine = new CommandLine(args);
    auto parameters = commandLine->parameters();
    delete commandLine;
    printf("%s\r\n", parameters);
    delete parameters;
    return 0;
}

#endif //AVR_ECHO_H
