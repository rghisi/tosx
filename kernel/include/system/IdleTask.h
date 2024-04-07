//
// Created by ghisi on 01.04.24.
//

#ifndef AVR_IDLETASK_H
#define AVR_IDLETASK_H


#include "Task.h"

class IdleTask: public Task {
    void execute() override;
};


#endif //AVR_IDLETASK_H
