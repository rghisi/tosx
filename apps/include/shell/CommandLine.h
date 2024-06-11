//
// Created by ghisi on 09.03.24.
//

#ifndef AVR_COMMANDLINE_H
#define AVR_COMMANDLINE_H


#include "cstdint"

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

#endif //AVR_COMMANDLINE_H
