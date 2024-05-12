//
// Created by ghisi on 09.03.24.
//

#include "shell/ExecutableFile.h"

#include "cstdio"

ExecutableFile::ExecutableFile(const char *name, int_fast8_t (*entryPoint)(char *)) {
    this->name = name;
    this->entryPointFunction = entryPoint;
}