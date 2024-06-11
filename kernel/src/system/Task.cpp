//
// Created by ghisi on 09.03.24.
//

#include "system/Task.h"
#include "system/TaskState.h"
#include "system/StaticStack.h"

Task::Task() {
    this->stack = new StaticStack<256>();
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

void Task::ready() {
    taskState = TaskState::READY;
}

void Task::terminated() {
  taskState = TaskState::TERMINATED;
}

bool Task::isRunning() {
  return taskState == TaskState::RUNNING;
}

bool Task::isCreated() {
  return taskState == TaskState::CREATED;
}

bool Task::isTerminated() {
  return taskState == TaskState::TERMINATED;
}

bool Task::isBlocked() {
  return taskState == TaskState::BLOCKED;
}

//TaskStats Task::stats() {
//    return taskStats;
//}
