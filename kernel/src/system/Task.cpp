//
// Created by ghisi on 09.03.24.
//

#include "system/Task.h"
#include "system/TaskState.h"
#include "system/StaticStack.h"

uint_fast16_t Task::nextId = 0;

Task::Task() {
    this->stack = &stackStack;
    id = nextId++;
}

Task::~Task() {
    delete stack;
}

void Task::initialize() {

}

void Task::block() {
    taskState = TaskState::BLOCKED;
}

void Task::running() {
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

const char *Task::Name() {
    return name;
}

uint_fast16_t Task::Id() {
    return id;
}
