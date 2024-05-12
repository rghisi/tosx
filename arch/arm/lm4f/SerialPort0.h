//
// Created by ghisi on 09.11.22.
//
// send char to console etc.
#ifndef ARM_LM4F_SERIALPORT0_H
#define ARM_LM4F_SERIALPORT0_H

#include "cstdint"
#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/uart.h>
#include "comms/USART.h"

class ReadCharPromise: public PromiseWithReturn<char> {
 public:
  bool isCompleted() override {
    auto received = (UART_FR(UART0) & UART_FR_RXFF) > 0;
    if (received) {
      this->data = uart_recv(UART0);
    }

    return received;
  }
};

class SerialPort0: public USART {
public:
    void setup();
    void send(uint8_t byte) override;
    PromiseWithReturn<char> *readCharAsync() override;
};

void SerialPort0::send(uint8_t byte) {
  uart_send_blocking(UART0, byte);
}

PromiseWithReturn<char> *SerialPort0::readCharAsync() {
  UART_ECR(UART0) = 0xFF;
  return new ReadCharPromise();
}

void SerialPort0::setup() {
  periph_clock_enable(RCC_GPIOA);
  gpio_set_af(GPIOA, 1, GPIO0 | GPIO1);
  periph_clock_enable(RCC_UART0);
  __asm__("nop");
  uart_disable(UART0);
  uart_clock_from_piosc(UART0);
  uart_set_baudrate(UART0, 921600);
  uart_set_databits(UART0, 8);
  uart_set_parity(UART0, UART_PARITY_NONE);
  uart_set_stopbits(UART0, 1);
  uart_set_flow_control(UART0, uart_flowctl::UART_FLOWCTL_NONE);
  uart_enable(UART0);
}
#endif //ARM_LM4F_SERIALPORT0_H
