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
    bin->Link(new ExecutableFile("ls", &(List::run)));
    bin->Link(new ExecutableFile("echo", &(Echo::run)));
    bin->Link(new ExecutableFile("free", &(Free::run)));
    bin->Link(new ExecutableFile("pi", &(PiTask::run)));
    bin->Link(new ExecutableFile("long", &(LongTask::run)));
    bin->Link(new ExecutableFile("clear", &(Clear::run)));
    bin->Link(new ExecutableFile("cdn", &CountDown::run));
    bin->Link(new ExecutableFile("uptime", &(Uptime::run)));
    bin->Link(new ExecutableFile("null", &(Null::run)));
    bin->Link(new ExecutableFile("memtest", &(MemTest::run)));
    bin->Link(new ExecutableFile("cputest", &(CpuTest::run)));
    bin->Link(new ExecutableFile("memstatus", &(MemStatus::run)));
    bin->Link(new ExecutableFile("random", &(RandomTest::run)));
}

void Init::StartShell() {
    printf("\tShell\r\n");
    auto task = OS::createTask("Shell", Shell::run, nullptr);
    OS::schedule(task);
}


#endif //TOSX_INIT_H
