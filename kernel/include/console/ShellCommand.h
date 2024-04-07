//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_SHELLCOMMAND_H
#define AVR_SHELLCOMMAND_H

class ShellCommand {
public:
    explicit ShellCommand(const char *name) {
        this->name = name;
    }
    virtual void run(char* args) = 0;
    const char *name;
};

#endif //AVR_SHELLCOMMAND_H
