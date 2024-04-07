//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_LIST_H
#define AVR_LIST_H

#include "shell/Shell.h"
#include "shell/ShellCommand.h"
#include "comms/Serial.h"

class List: public ShellCommand {
public:
    explicit List(Shell *shell) : ShellCommand("ls") {
        this->shell = shell;
    }

    void run(char* args) override {
        for (auto &command: shell->commands) {
            Serial::send(command->name);
            Serial::send("\t");
        }
        for (auto &app: shell->apps) {
            Serial::send(app->name);
            Serial::send("\t");
        }
        Serial::send("\r\n");
    }

private:
    Shell *shell;
};
#endif //AVR_LIST_H
