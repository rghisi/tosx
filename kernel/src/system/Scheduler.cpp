//
// Created by ghisi on 13.10.22.
//

#include "cstdio"
#include "cstring"
#include "system/Scheduler.h"
#include "system/TaskState.h"
#include "system/OS.h"

void Scheduler::schedule(Task *task) {
    scheduledTasks->push(task);
}

void Scheduler::add(Task *task, Promise *promise) {
    taskPromises->push(new TaskPromise(task, promise));
}

void Scheduler::processPromises() {
    auto promises = taskPromises->size();
    for (size_t i = 0; i < promises; i++) {
        auto taskPromise = taskPromises->pop();
        if (taskPromise->promise->isCompleted()) {
            taskPromise->task->waiting();
            delete taskPromise;
        } else {
            taskPromises->push(taskPromise);
        }
    }
}

void Scheduler::processRegularTasks() {
    if (!scheduledTasks->isEmpty()) {
        auto *task = scheduledTasks->pop();
        switch (task->state()) {
            case TaskState::CREATED: {
                scheduledTasks->push(task);
                OS::switchToTask(task);
            }
            case TaskState::WAITING: {
                scheduledTasks->push(task);
                OS::switchToTask(task);
            }
            case TaskState::RUNNING: {
                scheduledTasks->push(task);
                OS::switchToTask(idleTask);
            }
            case TaskState::BLOCKED: {
                scheduledTasks->push(task);
                OS::switchToTask(idleTask);
            }
            case TaskState::TERMINATED: {
                delete task;
                OS::switchToTask(idleTask);
            }
        }
    }
}

void Scheduler::execute() {
    processPromises();
    processRegularTasks();
}
