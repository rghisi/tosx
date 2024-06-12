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
    std::string_view command();
    std::string_view parameter(uint_fast16_t index);
    bool endsWith(char c);
    std::string_view parameters();
    bool hasParameters();

private:
    const char* args;
    uint_fast8_t NumberOfParameters();
};

#endif //AVR_COMMANDLINE_H
