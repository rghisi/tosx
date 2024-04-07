//
// Created by ghisi on 09.03.24.
//

#ifndef AVR_COMMANDLINE_H
#define AVR_COMMANDLINE_H


#include "cstdint"
#include "../std/String.h"

class CommandLine {
public:
    explicit CommandLine(const char* args);
    ~CommandLine();
    char* command();
    char* parameter(uint_fast16_t index);
    bool hasParameter(const char* paramName);
    bool endsWith(char c);
    char *parameters();

private:
    char** parts;
    uint_fast16_t numberOfParameters;
};

CommandLine::CommandLine(const char *args) {
    parts = String::split(' ', args);
    numberOfParameters = String::count(' ', args) + 1;
}

char *CommandLine::command() {
    return parts[0];
}

char *CommandLine::parameter(uint_fast16_t index) {
    return parts[index + 1];
}

bool CommandLine::hasParameter(const char *paramName) {
    for (size_t i = 1; i < numberOfParameters; i++) {
        bool result = true;
        auto part = parts[i];
        size_t j = 0;
        while (paramName[j] != 0 && part[j] != 0) {
            result &= paramName[j] == part[j];
            j++;
        }
        result &= paramName[j - 1] != 0;
        result &= part[j - 1] != 0;
        if (result) {
            return true;
        }
    }

    return false;
}

CommandLine::~CommandLine() {
    for (size_t i = 0; i < numberOfParameters; i++) {
        delete parts[i];
    }
    delete parts;
}

bool CommandLine::endsWith(char c) {
    auto lastPart = parts[numberOfParameters - 1];

    return lastPart[0] == c;
}

char *CommandLine::parameters() {
    size_t length = numberOfParameters;
    for (size_t i = 1; i < numberOfParameters; i++) {
        length += strlen(parts[i]);
    }

    auto result = new char[length];
    if (length > 1) {
        size_t ri = 0;
        for (size_t i = 1; i < numberOfParameters; i++) {
            auto part = parts[i];
            size_t j = 0;
            while (part[j] != 0) {
                result[ri] = part[j];
                j++;
                ri++;
            }
            result[ri] = ' ';
            ri++;
        }
        result[--ri] = 0;
    } else {
        result[0] = 0;
    }

    return result;
}


#endif //AVR_COMMANDLINE_H
