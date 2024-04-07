//
// Created by ghisi on 05.04.24.
//

#include "system/TaskStats.h"
#include "system/OS.h"

void TaskStats::record(TaskState before, TaskState after) {
    auto now = OS::now();
    counters[static_cast<int>(before)] += now - timestamp;
    timestamp = now;
}
