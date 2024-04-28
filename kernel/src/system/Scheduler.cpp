//
// Created by ghisi on 13.10.22.
//

#include "system/Scheduler.h"

#include "comms/Serial.h"
#include "cstdio"
#include "cstring"
#include "system/OS.h"
#include "system/TaskState.h"

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
      taskPromise->task->ready();
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
      case TaskState::READY: {
//        Serial::send('Y');
        scheduledTasks->push(task);
        task->running();
        OS::switchToTask(task);
        return;
      }
      case TaskState::RUNNING: {
//        Serial::send('U');
        scheduledTasks->push(task);
        task->ready();
        OS::switchToTask(idleTask);
        return;
      }
      case TaskState::BLOCKED: {
//        Serial::send('B');
        scheduledTasks->push(task);
        OS::switchToTask(idleTask);
        return;
      }
      case TaskState::TERMINATED: {
//        Serial::send('T');
        delete task;
        OS::switchToTask(idleTask);
        return;
      }
      default: {
//        Serial::send('D');
        scheduledTasks->push(task);
        OS::switchToTask(idleTask);
        return;
      }
    }
  } else {
    Serial::send('E');
    OS::switchToTask(idleTask);
  }
}

void Scheduler::execute() {
  while (true) {
//    Serial::send('K');
    processPromises();
    processRegularTasks();
  }
}

void Scheduler::setIdleTask(Task *idleTask) {
  this->idleTask = idleTask;
}
