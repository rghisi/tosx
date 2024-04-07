//
// Created by ghisi on 09.03.24.
//

#include "system/Task.h"
#include "system/TaskState.h"
#include "system/StaticStack.h"

Task::Task() {
    this->stack = new StaticStack<64>();
}

Task::~Task() {
    delete stack;
}

void Task::initialize() {
    //reset memory contents
}

void Task::block() {
    taskState = TaskState::BLOCKED;
}

void Task::running() {
//    taskStats.record(this->taskState, TaskState::RUNNING);
    taskState = TaskState::RUNNING;
}

TaskState Task::state() {
    return taskState;
}

void Task::waiting() {
    taskState = TaskState::WAITING;
}

//TaskStats Task::stats() {
//    return taskStats;
//}
