//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_AVRCPU_H
#define AVR_AVRCPU_H

#include <avr/interrupt.h>
#include "system/Cpu.h"
#include "system/OS.h"
#include "system/Task.h"
#include "AVRContextSwitching.h"

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

void AVRCpu::enableInterrupts() {
    sei();
}

void AVRCpu::disableInterrupts() {
    cli();
}

void AVRCpu::setupSysTicks() {
    size_t a = 10;
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS01) | _BV(CS00);
    TCNT0 = 0;
    OCR0A = 249;
    TIMSK0 = _BV(OCIE0A);
}

void AVRCpu::startTask(Task *task) {
    cli();
    SP = task->stack->pointer;
    sei();
    task->execute();
    switchToKernel(task);
}

void AVRCpu::switchToTask(Task *task) {
    cli();
    SP = task->stack->pointer;
    restoreContext();
}

void AVRCpu::startKernelTask(Task *kernelTask) {
    this->kernelTask = kernelTask;
    cli();
    SP = kernelTask->stack->pointer;
    sei();
    kernelTask->execute();
}

void AVRCpu::switchToKernel(Task *task) {
    cli();
    saveContext();
    task->stack->pointer = SP;
    sei();
    SP = kernelTask->stack->pointer;
    kernelTask->execute();
}

uint8_t AVRCpu::acc = 0;

extern "C" {
void TIMER0_COMPA_vect(void) {
    OS::incrementTick();
    AVRCpu::acc++;
    if (AVRCpu::acc == 10) {
        AVRCpu::acc = 0;
        OS::preempt();
    }
}
}


/**
 * __attribute__ ((naked)) static void switchToTaskNaked(Task *task) {

    }
 */


#endif //AVR_AVRCPU_H
