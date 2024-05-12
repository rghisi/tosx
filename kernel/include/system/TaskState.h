//
// Created by ghisi on 05.04.24.
//

#ifndef AVR_TASKSTATE_H
#define AVR_TASKSTATE_H

#include <cstdint>

enum class TaskState: uint_fast8_t {
    CREATED,
  READY, RUNNING, BLOCKED, TERMINATED
};

#endif //AVR_TASKSTATE_H
