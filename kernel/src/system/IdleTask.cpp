//
// Created by ghisi on 04.04.24.
//

#include "system/IdleTask.h"

#include "comms/Serial.h"

void IdleTask::execute() {
    while (true) {
//      Serial::send('i');
    }
}
#include "system/StaticStack.h"

IdleTask::IdleTask() {
    name = "Idle";
    stack = std::make_unique<StaticStack<128>>();
}
