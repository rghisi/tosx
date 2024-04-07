//
// Created by ghisi on 05.04.24.
//

#ifndef AVR_TASKSTATS_H
#define AVR_TASKSTATS_H

#include "TaskState.h"

class TaskStats {
public:
    void record(TaskState before, TaskState after);
private:
    uint32_t timestamp = 0;
    uint32_t counters[6];
};


#endif //AVR_TASKSTATS_H
