//
// Created by ghisi on 5/11/24.
//

#ifndef TOSX_LM4FCPU_H
#define TOSX_LM4FCPU_H

#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/rcc.h>

#include "generic/CortexM4Cpu.h"

class Lm4fCpu: public CortexM4Cpu {
 public:
  void setup() override;
  void setupSysTicks() override;
 private:
  static constexpr uint32_t PLL_DIV_80MHZ = 5;
};

void Lm4fCpu::setup() {
  rcc_sysclk_config(OSCSRC_MOSC, XTAL_16M, PLL_DIV_80MHZ);
  gpio_enable_ahb_aperture();
  CortexM4Cpu::setup();
}

void Lm4fCpu::setupSysTicks() {
  /** Needs calibration */
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
  systick_set_reload(3999);
  systick_interrupt_enable();
  systick_counter_enable();
}

#endif  // TOSX_LM4FCPU_H
