//
// Created by ghisi on 09.11.22.
//
// send char to console etc.
#ifndef ARM_LM4F_SERIALPORT0_H
#define ARM_LM4F_SERIALPORT0_H

#include "cstdint"
#include "vector"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/g4/nvic.h>
#include "comms/USART.h"

//extern "C" void lpuart_isr(void);

class ReadCharPromise: public PromiseWithReturn<char> {
 public:
  bool isCompleted() override {
    auto received = (LPUART1_ISR & USART_FLAG_RXNE) > 0;
    if (received) {
      this->data = usart_recv(LPUART1);
    }

    return received;
  }
};

class SerialPort0: public USART {
public:
    SerialPort0();
    void setup();
    void send(uint8_t byte) override;
    PromiseWithReturn<char> *readCharAsync() override;
    void disableTransmitter() override;
    void enableTransmitter() override;
};

SerialPort0::SerialPort0() = default;

void SerialPort0::disableTransmitter() {
}

void SerialPort0::enableTransmitter() {
}

void SerialPort0::send(uint8_t byte) {
  usart_send_blocking(LPUART1_BASE, static_cast<uint8_t>(byte));
}

PromiseWithReturn<char> *SerialPort0::readCharAsync() {
  LPUART1_RQR = USART_RQR_RXFRQ;
  LPUART1_ICR = USART_ICR_ORECF;
  auto promise = new ReadCharPromise();

  return promise;
}

void SerialPort0::setup() {
  nvic_enable_irq(NVIC_LPUART_IRQ);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_LPUART1);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
  gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);
  gpio_set_af(GPIOA, GPIO_AF12, GPIO2);
  gpio_set_af(GPIOA, GPIO_AF12, GPIO3);


  usart_set_baudrate(LPUART1, 921600);
  usart_set_databits(LPUART1, 8);
  usart_set_stopbits(LPUART1, USART_STOPBITS_1);
  usart_set_mode(LPUART1, USART_MODE_TX_RX);
  usart_set_parity(LPUART1, USART_PARITY_NONE);
  usart_set_flow_control(LPUART1, USART_FLOWCONTROL_NONE);

  /* Finally enable the USART. */
  usart_enable(LPUART1);
}

#endif //ARM_LM4F_SERIALPORT0_H
