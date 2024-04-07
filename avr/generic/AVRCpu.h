//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_AVRCPU_H
#define AVR_AVRCPU_H

#include <avr/interrupt.h>
#include "system/Cpu.h"
#include "system/Task.h"

extern "C" void TIMER0_COMPA_vect(void) __attribute__ ((signal));

class AVRCpu: public Cpu {
public:
    void enableInterrupts() override;
    void disableInterrupts() override;
    void setupSysTicks() override;
    void startTask(Task *task) override;
    void switchToTask(Task *task) override;
    void startKernelTask(Task *kernelTask) override;
    void switchToKernel(Task *task) override;
    static uint8_t acc;
private:
    Task* kernelTask = nullptr;
};

/**
 * __attribute__ ((naked)) static void switchToTaskNaked(Task *task) {

    }
 */


#endif //AVR_AVRCPU_H
