//
// Created by ghisi on 03.11.23.
//

#ifndef AVR_LONGTASK_H
#define AVR_LONGTASK_H

#include <cstring>
#include <cstdio>
#include "comms/Serial.h"
#include "system/Task.h"
#include "system/OS.h"

class LongTask {
public:
    static int_fast8_t run(char* args);
};

int_fast8_t LongTask::run(char *args) {
    printf("Starting a long task that will take 10s to run...\r\n");
    OS::sleep(10000);
    printf("Long Task terminated.\r\n");

    return 0;
}

#endif //AVR_LONGTASK_H
