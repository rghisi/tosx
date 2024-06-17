//
// Created by ghisi on 11.06.24.
//

#ifndef TOSX_ROMFILESYSTEM_H
#define TOSX_ROMFILESYSTEM_H


#include <cstdio>
#include "fs/FileSystem.h"
#include "fs/Dir.h"
#include "shell/ExecutableFile.h"
#include "misc/List.h"
#include "misc/Echo.h"
#include "misc/Free.h"
#include "misc/PiTask.h"
#include "misc/LongTask.h"
#include "misc/Clear.h"
#include "misc/CountDown.h"
#include "misc/Uptime.h"
#include "misc/Null.h"
#include "misc/MemTest.h"
#include "misc/CpuTest.h"
#include "misc/MemStatus.h"
#include "misc/RandomTest.h"
#include "misc/Ps.h"

class RomFileSystem: public FileSystem {
public:
    void Mount(Dir *mountPoint) override;

};

void RomFileSystem::Mount(Dir *mountPoint) {
    printf("\tROM FS\r\n");
    mountPoint->Link(new ExecutableFile("ls", &List::run));
    mountPoint->Link(new ExecutableFile("echo", &Echo::run));
    mountPoint->Link(new ExecutableFile("free", &Free::run));
    mountPoint->Link(new ExecutableFile("pi", &PiTask::run));
    mountPoint->Link(new ExecutableFile("long", &LongTask::run));
    mountPoint->Link(new ExecutableFile("clear", &Clear::run));
    mountPoint->Link(new ExecutableFile("cdn", &CountDown::run));
    mountPoint->Link(new ExecutableFile("uptime", &Uptime::run));
    mountPoint->Link(new ExecutableFile("null", &Null::run));
    mountPoint->Link(new ExecutableFile("memtest", &MemTest::run));
    mountPoint->Link(new ExecutableFile("cputest", &CpuTest::run));
    mountPoint->Link(new ExecutableFile("memstatus", &MemStatus::run));
    mountPoint->Link(new ExecutableFile("random", &RandomTest::run));
    mountPoint->Link(new ExecutableFile("ps", &Ps::run));
}


#endif //TOSX_ROMFILESYSTEM_H
