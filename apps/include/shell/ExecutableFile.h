//
// Created by ghisi on 18.11.23.
//

#ifndef AVR_EXECUTABLEFILE_H
#define AVR_EXECUTABLEFILE_H

#include "cstdint"
#include "fs/Node.h"

class ExecutableFile: public Node {
public:
    ExecutableFile(const char *name, int_fast8_t (*entryPoint)(char *));
    const char* name;
    int_fast8_t (*entryPointFunction)(char *);
};

#endif //AVR_EXECUTABLEFILE_H
