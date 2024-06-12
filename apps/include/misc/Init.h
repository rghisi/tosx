//
// Created by ghisi on 10.06.24.
//

#ifndef TOSX_INIT_H
#define TOSX_INIT_H


#include "fs/FileSystemService.h"
#include "shell/ExecutableFile.h"
#include "Echo.h"
#include "Free.h"
#include "LongTask.h"
#include "Clear.h"
#include "CountDown.h"
#include "Uptime.h"
#include "Null.h"
#include "MemTest.h"
#include "CpuTest.h"
#include "MemStatus.h"
#include "RandomTest.h"
#include "PiTask.h"
#include "shell/Shell.h"
#include "List.h"
#include "fs/RomFileSystem.h"

class Init {
public:
    static int_fast8_t run(char* args);
private:
    static void MountRoot();
    static void StartShell();
};

int_fast8_t Init::run(char* args) {
    printf("Init...\r\n");
    MountRoot();
    StartShell();
    return 0;
}

void Init::MountRoot() {
    printf("\tRoot FS\r\n");
    auto fs = FileSystemService::instance();
    auto bin = fs->getDir("/")->mkdir("bin");
    fs->Mount(new RomFileSystem(), bin, FileSystemService::MountOption::READ_ONLY);
}

void Init::StartShell() {
    printf("\tShell\r\n");
    auto task = OS::createTask("Shell", Shell::run, nullptr);
    OS::schedule(task);
}


#endif //TOSX_INIT_H
