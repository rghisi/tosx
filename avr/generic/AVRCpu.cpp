#include "AVRCpu.h"
#include "AVRContextSwitching.h"
#include "system/OS.h"

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
