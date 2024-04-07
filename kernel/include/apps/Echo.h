//
// Created by ghisi on 08.03.24.
//

#ifndef AVR_ECHO_H
#define AVR_ECHO_H

#include "cstdint"
#include "../comms/Serial.h"
#include "../console/CommandLine.h"


class Echo {
public:
    static int_fast8_t run(char* args);
private:
    static constexpr const char* NEW_LINE = "\n\r";
};

int_fast8_t Echo::run(char *args) {
    auto commandLine = new CommandLine(args);
    auto parameters = commandLine->parameters();
    delete commandLine;
    Serial::send(parameters);
    Serial::send(NEW_LINE);
    delete parameters;
    return 0;
}

#endif //AVR_ECHO_H
