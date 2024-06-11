//
// Created by ghisi on 08.11.23.
//

#ifndef AVR_SHELL_H
#define AVR_SHELL_H

#include "ShellCommand.h"
#include "ExecutableFile.h"
#include "array"
#include "fs/Dir.h"

class Shell {
public:
    Shell();
    [[noreturn]] void run();
    static int_fast8_t run(char *args);
private:
    static constexpr uint_fast8_t COLUMNS = 40;
    static constexpr uint_fast8_t FIRST = 0;
    static constexpr uint_fast8_t LAST = COLUMNS - 1;
    static constexpr auto BACKSPACE = '\b';
    static constexpr auto CARRIAGE_RETURN = '\r';
    static constexpr const auto LINE_FEED = "\n";
    static constexpr const auto ERASE = "\033[J";
    static constexpr const auto MOVE_ONE_RIGHT = "\033[1C";
    static constexpr const auto COLOR_WHITE_BRIGHT = "\033[1;97m";
    static constexpr const auto COLOR_RED_BRIGHT = "\033[1;91m";
    static constexpr const auto RESET_STYLE = "\033[0m";
    static constexpr const auto START_DIR = "/";
    uint_fast8_t column = 0;
    bool prompt = true;
    char line[COLUMNS] = {};
    void resetLine();
    void executeLineHandler();
    void executeForeground(const char *name, int_fast8_t (*entryPoint)(char *), char *args);
    void executeBackground(const char *name, int_fast8_t (*entryPoint)(char *), char *args);
};

#endif //AVR_SHELL_H
