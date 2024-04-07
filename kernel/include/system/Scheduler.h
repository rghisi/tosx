//
// Created by ghisi on 13.10.22.
//

#ifndef AVR_SCHEDULER_H
#define AVR_SCHEDULER_H


#include "cstdint"
#include "Task.h"
#include "collections/StaticPriorityQueue.h"
#include "PeriodicTask.h"
#include "collections/LinkedList.h"
#include "Promise.h"
#include "collections/CircularQueue.h"
#include "IdleTask.h"

class TaskPromise {
public:
    TaskPromise(Task *task, Promise *promise) {
        this->task = task;
        this->promise = promise;
    }
    ~TaskPromise() = default;
    Task *task;
    Promise *promise;
};

class PeriodicScheduledTask {
public:
    explicit PeriodicScheduledTask(PeriodicTask *task) {
        this->task = task;
        nextExecution = 0;
    }
    ~PeriodicScheduledTask() = default;

    bool operator<(const PeriodicScheduledTask &rhs) const {
        return nextExecution < rhs.nextExecution;
    }

    bool operator>(const PeriodicScheduledTask &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const PeriodicScheduledTask &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const PeriodicScheduledTask &rhs) const {
        return !(*this < rhs);
    }

    PeriodicTask *task;
    volatile uint_fast32_t nextExecution;
};

class Scheduler: public Task {
public:
    void execute() override;
    void schedule(Task *task);
    void add(Task *task, Promise *promise);
private:
    FifoQueue<Task*>* scheduledTasks = new CircularQueue<Task*, 5>();
    FifoQueue<TaskPromise*>* taskPromises = new CircularQueue<TaskPromise*, 5>();
    IdleTask* idleTask = new IdleTask();
    void processPromises();
    void processRegularTasks();
};


#endif //AVR_SCHEDULER_H
