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

extern "C" void lpuart_isr(void);

class SerialPort0: public USART {
public:
    SerialPort0();
    void setup();
    void send(uint8_t byte) override;
    void readCharAsync(PromiseWithReturn<char> *promise) override;
    void disableTransmitter() override;
    void enableTransmitter() override;
    friend void lpuart_isr(void);

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
  usart_send_blocking(LPUART1_BASE, static_cast<uint8_t>(byte));
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
  usart_enable_rx_interrupt(LPUART1);
}

void SerialPort0::disableReceiveInterrupt() {
  usart_disable_rx_interrupt(LPUART1);
}

void SerialPort0::setup() {
  nvic_enable_irq(NVIC_LPUART_IRQ);
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

void lpuart_isr(void) {
  /* Check if we were called because of RXNE. */
  if (((USART_CR1(LPUART1) & USART_CR1_RXNEIE) != 0)) {
    auto it = SerialPort0::it;
    if (it != nullptr && it->readAsyncState == SerialPort0::ReadAsyncState::WAITING_RECEIVE) {
      it->readAsyncPromise->data = usart_recv(LPUART1);
      it->readAsyncPromise->complete();
      it->readAsyncState = SerialPort0::ReadAsyncState::DISCONNECTED;
    }
    SerialPort0::disableReceiveInterrupt();
    SerialPort0::disableReceiver();
  }
//  usart_clear_interrupt_flag(USART1, static_cast<usart_interrupt_flag>(0xFF));
}


#endif //ARM_LM4F_SERIALPORT0_H
