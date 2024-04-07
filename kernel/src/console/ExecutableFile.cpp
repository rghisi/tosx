//
// Created by ghisi on 09.03.24.
//

#include "console/ExecutableFile.h"

ExecutableFile::ExecutableFile(const char *name, int_fast8_t (*entryPoint)(char *)) {
    this->name = name;
    this->entryPointFunction = entryPoint;
}