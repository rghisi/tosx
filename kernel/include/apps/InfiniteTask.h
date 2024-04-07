//
// Created by ghisi on 11.03.23.
//

#ifndef AVR_INFINITETASK_H
#define AVR_INFINITETASK_H


#include "../system/Task.h"
#include "../system/StaticStack.h"

class InfiniteTask: public Task {
public:
    explicit InfiniteTask(uint8_t taskNumber);
    void execute() override;
private:
    StaticStack<64> stack = StaticStack<64>();
    uint8_t taskNumber;
    void print(uint16_t counter, uint8_t i);
    void printMessage(uint16_t counter, uint8_t i);
};


#endif //AVR_INFINITETASK_H
