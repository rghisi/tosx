//
// Created by ghisi on 13.10.22.
//

#include "system/Scheduler.h"

#include <memory>

#include "cstdio"
#include "system/OS.h"
#include "system/TaskState.h"

Scheduler::Scheduler() {
  name = "Kernel";
}

void Scheduler::schedule(Task *task) {
  scheduledTasks.push(task);
}

void Scheduler::add(Task *task, Promise *promise) {
  taskPromises.push(new TaskPromise(task, promise));
}

void Scheduler::processPromises() {
  auto promises = taskPromises.size();
  for (size_t i = 0; i < promises; i++) {
    auto taskPromise = taskPromises.pop();
    if (taskPromise->promise->isCompleted()) {
      taskPromise->task->ready();
      delete taskPromise;
    } else {
      taskPromises.push(taskPromise);
    }
  }
}

void Scheduler::processRegularTasks() {
  if (!scheduledTasks.isEmpty()) {
    auto size = scheduledTasks.size();
    for (size_t i = 0; i < size; i++) {
      auto *task = scheduledTasks.pop();
      switch (task->state()) {
        case TaskState::READY: {
          task->running();
          scheduledTasks.push(task);
          OS::switchToTask(task);
          return;
        }
        case TaskState::RUNNING: {
          task->ready();
          scheduledTasks.push(task);
          break;
        }
        case TaskState::BLOCKED: {
          scheduledTasks.push(task);
          break;
        }
        case TaskState::TERMINATED: {
          delete task;
          break;
        }
        default: {
          scheduledTasks.push(task);
          break;
        }
      }
    }
  }
  OS::switchToTask(idleTask);
}

void Scheduler::execute() {
  while (true) {
    processPromises();
    processRegularTasks();
  }
}

void Scheduler::setIdleTask(Task *idleTask) {
  this->idleTask = idleTask;
}
