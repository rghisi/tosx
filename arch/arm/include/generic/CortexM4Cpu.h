//
// Created by ghisi on 11.03.24.
//

#ifndef ARM_CORTEX_M4_CPU_H
#define ARM_CORTEX_M4_CPU_H

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "system/Cpu.h"
#include "system/OS.h"
#include "system/Task.h"

volatile uintptr_t *stackPointerToSave asm("stackPointerToSave")
    __attribute__((used));
volatile uintptr_t *stackPointerToRestore asm("stackPointerToRestore")
    __attribute__((used));

extern "C" {
void sys_tick_handler(void);
void pend_sv_handler(void);
void execTask(Task *task);
};

extern "C" void execTask(Task *task) {
  task->execute();
  OS::terminate(task);
}

class CortexM4Cpu : public Cpu {
 public:
  void setup() override;
  void enableInterrupts() override;
  void disableInterrupts() override;
  void initialize(Task *task) override;
  void swapContext(uintptr_t *stackPointerToStore,
                   uintptr_t *stackPointerToLoad) override;
  friend void sys_tick_handler(void);
  friend void pend_sv_handler(void);

 private:
  static uint32_t heartBeatPrescaler;
  static uint32_t preemptionPrescaler;
  static constexpr uint32_t PLL_DIV_80MHZ = 5;
  static uintptr_t *alignDown(uintptr_t *ptr);
  static void triggerPendSV();
};

uint32_t CortexM4Cpu::preemptionPrescaler = 0;
uint32_t CortexM4Cpu::heartBeatPrescaler = 0;


void CortexM4Cpu::setup() {
  /* set the PendSV interrupt priority to the lowest level 0xFF */
  *(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);
}

void CortexM4Cpu::enableInterrupts() { cm_enable_interrupts(); }

void CortexM4Cpu::disableInterrupts() { cm_disable_interrupts(); }

void CortexM4Cpu::initialize(Task *task) {
  auto sp = alignDown(task->stack->pointer);
  *(sp--) = (1U << 24);          /* xPSR */
  *(sp--) = (uintptr_t)execTask; /* PC */
  *(sp--) = 0x0000000EU;         /* LR  */
  *(sp--) = 0x12121212U;         /* R12 */
  *(sp--) = 0x03030303U;         /* R3  */
  *(sp--) = 0x02020202U;         /* R2  */
  *(sp--) = 0x01010101U;         /* R1  */
  *(sp--) = (uintptr_t)task;     /* R0  */
  /* additionally, fake registers R4-R11 */
  *(sp--) = 0x11111111U; /* R11 */
  *(sp--) = 0x1010101AU; /* R10 */
  *(sp--) = 0x09090909U; /* R9 */
  *(sp--) = 0x08080808U; /* R8 */
  *(sp--) = 0x07070707U; /* R7 */
  *(sp--) = 0x06060606U; /* R6 */
  *(sp--) = 0x05050505U; /* R5 */
  *(sp) = 0x04040404U;   /* R4 */
  task->stack->pointer = sp;
}

void CortexM4Cpu::swapContext(uintptr_t *stackPointerToStore,
                              uintptr_t *stackPointerToLoad) {
  stackPointerToSave = stackPointerToStore;
  stackPointerToRestore = stackPointerToLoad;
  triggerPendSV();
}

uintptr_t *CortexM4Cpu::alignDown(uintptr_t *ptr) {
  auto addr = (uintptr_t)ptr;
  addr = addr & ~7U;
  return (uintptr_t *)addr;
}

void sys_tick_handler(void) {
  OS::incrementTick();
  CortexM4Cpu::preemptionPrescaler++;
  CortexM4Cpu::heartBeatPrescaler++;
  if (CortexM4Cpu::preemptionPrescaler == 10) {
    CortexM4Cpu::preemptionPrescaler = 0;
    OS::preempt();
  }
}

__attribute__((naked)) void pend_sv_handler(void) {
  __asm__ volatile(
      "CPSID   I                          \n"
      "PUSH    {r4-r11}                   \n"
      "LDR     r1,=stackPointerToSave     \n"
      "LDR     r1,[r1,#0x00]              \n"
      "CMP     r1,#0                      \n"
      "BEQ     load                       \n"
      "STR     sp,[r1,#0x00]              \n"
      "load:                              \n"
      "LDR     r1,=stackPointerToRestore  \n"
      "LDR     sp,[r1,#0x00]              \n"
      "POP     {r4-r11}                   \n"
      "CPSIE   I                          \n"
      "BX      lr                         \n");
}

inline void CortexM4Cpu::triggerPendSV() {
  *(uint32_t volatile *)0xE000ED04 = (1U << 28);
}

#endif  // ARM_CORTEX_M4_CPU_H
