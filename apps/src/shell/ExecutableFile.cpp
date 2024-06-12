//
// Created by ghisi on 09.03.24.
//

#include "shell/ExecutableFile.h"

ExecutableFile::ExecutableFile(const char *name, int_fast8_t (*entryPoint)(char *)) : Node(name) {
    this->name = name;
    this->entryPointFunction = entryPoint;
}