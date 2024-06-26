//
// Created by ghisi on 08.11.23.
//

#ifndef AVR_TASKCOMPLETIONPROMISE_H
#define AVR_TASKCOMPLETIONPROMISE_H

#include "Promise.h"
#include "Task.h"
#include "TaskState.h"

class TaskCompletionPromise: public Promise {
public:
    explicit TaskCompletionPromise(Task *task);
    ~TaskCompletionPromise() override;
    [[nodiscard]] bool isCompleted() override;
private:
    Task *task;
};

TaskCompletionPromise::TaskCompletionPromise(Task *task) {
    this->task = task;
}

bool TaskCompletionPromise::isCompleted() {
    return task->isTerminated();
}

TaskCompletionPromise::~TaskCompletionPromise() {

}

#endif //AVR_TASKCOMPLETIONPROMISE_H
