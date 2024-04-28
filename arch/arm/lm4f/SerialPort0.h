//
// Created by ghisi on 09.11.22.
//

#ifndef ARM_LM4F_SERIALPORT0_H
#define ARM_LM4F_SERIALPORT0_H

#include "cstdint"
#include "vector"
#include <libopencm3/lm4f/systemcontrol.h>
#include <libopencm3/lm4f/gpio.h>
#include <libopencm3/lm4f/uart.h>
#include <libopencm3/lm4f/nvic.h>
#include "comms/USART.h"

extern "C" void uart0_isr(void);

class SerialPort0: public USART {
public:
    SerialPort0();
    void setup();
    void send(uint8_t byte) override;
    void readCharAsync(PromiseWithReturn<char> *promise) override;
    void disableTransmitter() override;
    void enableTransmitter() override;
    friend void uart0_isr(void);

private:
    static SerialPort0 *it;
    enum class ReadAsyncState: uint_fast8_t { DISCONNECTED, WAITING_RECEIVE };
    ReadAsyncState readAsyncState = ReadAsyncState::DISCONNECTED;
    PromiseWithReturn<char> *readAsyncPromise = nullptr;
    static void disableReceiver();
    static void enableReceiver();
    static void enableReceiveInterrupt();
    static void disableReceiveInterrupt();
};

SerialPort0 *SerialPort0::it = nullptr;

SerialPort0::SerialPort0() {
  SerialPort0::it = this;
//  SerialPort0::it->enableTransmitter();
}

void SerialPort0::disableReceiver() {
}

void SerialPort0::enableReceiver() {
}

void SerialPort0::disableTransmitter() {
}

void SerialPort0::enableTransmitter() {
}

void SerialPort0::send(uint8_t byte) {
  uart_send_blocking(UART0, static_cast<uint8_t>(byte));
}

void SerialPort0::readCharAsync(PromiseWithReturn<char> *promise) {
  if (readAsyncState == ReadAsyncState::DISCONNECTED) {
    readAsyncState = ReadAsyncState::WAITING_RECEIVE;
    readAsyncPromise = promise;
    enableReceiver();
    enableReceiveInterrupt();
  } else {
    promise->complete();
  }
}

void SerialPort0::enableReceiveInterrupt() {
  uart_enable_rx_interrupt(UART0);
}

void SerialPort0::disableReceiveInterrupt() {
  uart_disable_rx_interrupt(UART0);
}

void SerialPort0::setup() {
  /* Enable GPIOA in run mode. */
  periph_clock_enable(RCC_GPIOA);
  /* Mux PA0 and PA1 to UART0 (alternate function 1) */
  gpio_set_af(GPIOA, 1, GPIO0 | GPIO1);

  /* Enable the UART clock */
  periph_clock_enable(RCC_UART0);
  /* We need a brief delay before we can access UART config registers */
  __asm__("nop");
  /* Disable the UART while we mess with its setings */
  uart_disable(UART0);
  /* Configure the UART clock source as precision internal oscillator */
  uart_clock_from_piosc(UART0);
  /* Set communication parameters */
  uart_set_baudrate(UART0, 921600);
  uart_set_databits(UART0, 8);
  uart_set_parity(UART0, UART_PARITY_NONE);
  uart_set_stopbits(UART0, 1);
  /* Now that we're done messing with the settings, enable the UART */
  uart_enable(UART0);

  /* Make sure the interrupt is routed through the NVIC */
  nvic_enable_irq(NVIC_UART0_IRQ);
}

void uart0_isr(void) {
  if (uart_is_interrupt_source(UART0, UART_INT_RX)) {
    auto it = SerialPort0::it;
    if (it != nullptr && it->readAsyncState == SerialPort0::ReadAsyncState::WAITING_RECEIVE) {
      it->readAsyncPromise->data = uart_recv(UART0);
      it->readAsyncPromise->complete();
      it->readAsyncState = SerialPort0::ReadAsyncState::DISCONNECTED;
    }
    SerialPort0::disableReceiveInterrupt();
    SerialPort0::disableReceiver();
  }
  uart_clear_interrupt_flag(UART0, static_cast<uart_interrupt_flag>(0xFF));
}


#endif //ARM_LM4F_SERIALPORT0_H
