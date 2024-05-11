//
// Created by ghisi on 11.03.24.
//

#ifndef ARM_STM32G474_CPU_H
#define ARM_STM32G474_CPU_H

#include "generic/CortexM4Cpu.h"

#include <libopencm3/stm32/g4/rcc.h>
#include "system/Cpu.h"
#include "system/OS.h"
#include "system/Task.h"

class Stm32g474 : public CortexM4Cpu {
 public:
  void setup() override;
  void setupSysTicks() override;
};

void Stm32g474::setup() {
  rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_170MHZ]);
  RCC_CRRCR |= 1;
  CortexM4Cpu::setup();
}

void Stm32g474::setupSysTicks() {
  /** Needs calibration */
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_set_reload(172500);
  systick_interrupt_enable();
  systick_counter_enable();
}

#endif  // ARM_STM32G474_CPU_H
