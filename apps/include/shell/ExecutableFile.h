//
// Created by ghisi on 18.11.23.
//

#ifndef AVR_EXECUTABLEFILE_H
#define AVR_EXECUTABLEFILE_H

#include "cstdint"

class ExecutableFile {
public:
    ExecutableFile(const char* name, int_fast8_t (*entryPoint)(char*));
    const char* name;
    int_fast8_t (*entryPointFunction)(char *);
};

#endif //AVR_EXECUTABLEFILE_H
