//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_AVRCPU_H
#define AVR_AVRCPU_H

#include <avr/interrupt.h>

#include "AVRContextSwitching.h"
#include "system/Cpu.h"
#include "system/OS.h"
#include "system/Task.h"

volatile uintptr_t *stackPointerToSave asm("stackPointerToSave")
    __attribute__((used));
volatile uintptr_t *stackPointerToRestore asm("stackPointerToRestore")
    __attribute__((used));

extern "C" {
void TIMER0_COMPA_vect(void) __attribute__((signal));
void execTask(Task *task);
__attribute__ ((naked)) void __swapContext();
};

class AVRCpu : public Cpu {
 public:
  void setup() override;
  void enableInterrupts() override;
  void disableInterrupts() override;
  void setupSysTicks() override;
  void initialize(Task *task) override;
  void swapContext(uintptr_t *stackPointerToStore,
                   uintptr_t *stackPointerToLoad) override;
  static uint8_t acc;

 private:
  static uintptr_t *alignDown(uintptr_t *ptr);
};

void AVRCpu::enableInterrupts() { sei(); }

void AVRCpu::disableInterrupts() { cli(); }

void AVRCpu::setupSysTicks() {
  TCCR0A = _BV(WGM01);
  TCCR0B = _BV(CS01) | _BV(CS00);
  TCNT0 = 0;
  OCR0A = 249;
  TIMSK0 = _BV(OCIE0A);
}

uint8_t AVRCpu::acc = 0;
void AVRCpu::swapContext(uintptr_t *stackPointerToStore,
                         uintptr_t *stackPointerToLoad) {
  stackPointerToSave = stackPointerToStore;
  stackPointerToRestore = stackPointerToLoad;
  ::__swapContext();
}
void AVRCpu::initialize(Task *task) {
  auto sp = alignDown(task->stack->pointer);
  auto execTaskAddress = (uintptr_t) execTask;
  execTaskAddress = (execTaskAddress << 8) | (execTaskAddress >> 8);
  auto taskAddressMsb = ((uintptr_t) task) & 0xFF00 ;
  auto taskAddressLsb = (uintptr_t) task & 0x00FF;
  *(sp--) = execTaskAddress;
  *(sp--) = 0x0000;
  *(sp--) = 0x0002;
  *(sp--) = 0x0304;
  *(sp--) = 0x0506;
  *(sp--) = 0x0708;
  *(sp--) = 0x0910;
  *(sp--) = 0x1112;
  *(sp--) = 0x1314;
  *(sp--) = 0x1516;
  *(sp--) = 0x1718;
  *(sp--) = 0x1920;
  *(sp--) = 0x2122;
  *(sp--) = 0x2300 | taskAddressLsb;
  *(sp--) = taskAddressMsb | 0x26;
  *(sp--) = 0x2728;
  *(sp--) = 0x2930;
  *(sp) = 0x3199;

  task->stack->pointer = sp;
}
void AVRCpu::setup() {

}

uintptr_t *AVRCpu::alignDown(uintptr_t *ptr) {
  auto addr = (uintptr_t)ptr;
  addr = addr & ~1U;
  return (uintptr_t *)addr;
}

__attribute__ ((naked)) void __swapContext() {
  if (stackPointerToSave != nullptr) {
    saveContext();
  }
  restoreContext();
}

void execTask(Task *task) {
  task->execute();
  OS::terminate(task);
}

extern "C" {
void TIMER0_COMPA_vect(void) {
  OS::incrementTick();
  AVRCpu::acc++;
  if (AVRCpu::acc == 100) {
    AVRCpu::acc = 0;
    OS::preempt();
  }
}
}

#endif  // AVR_AVRCPU_H
