//
// Created by ghisi on 09.11.22.
//

#ifndef AVR_SERIALPORT0_H
#define AVR_SERIALPORT0_H

#include "cstdint"
#include <avr/interrupt.h>
#include "comms/USART.h"

extern "C" void USART_RX_vect(void) __attribute__ ((signal));

class SerialPort0: public USART {
public:
    void setup();
    void send(uint8_t byte) override;
    PromiseWithReturn<char> *readCharAsync() override;
    friend void USART_RX_vect(void);

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

void SerialPort0::setup() {
  UCSR0C = (_BV(UCSZ01)) | (_BV(UCSZ00));
  UBRR0 = 0; //1mbps
  UCSR0B |= _BV(TXEN0);
  SerialPort0::it = this;
}

void SerialPort0::disableReceiver() {
  UCSR0B &= ~(_BV(RXEN0));
}

void SerialPort0::enableReceiver() {
  UCSR0B |= _BV(RXEN0);
}

void SerialPort0::send(uint8_t byte) {
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = byte;
}

PromiseWithReturn<char> *SerialPort0::readCharAsync() {
  if (readAsyncState == ReadAsyncState::DISCONNECTED) {
    readAsyncState = ReadAsyncState::WAITING_RECEIVE;
    readAsyncPromise = new PromiseWithReturn<char>();
    enableReceiver();
    enableReceiveInterrupt();
  }

  return readAsyncPromise;
}

void SerialPort0::enableReceiveInterrupt() {
  UCSR0B |= _BV(RXCIE0);
}

void SerialPort0::disableReceiveInterrupt() {
  UCSR0B &= ~(_BV(RXCIE0));
}

void USART_RX_vect(void) {
  auto it = SerialPort0::it;
  if (it != nullptr && it->readAsyncState == SerialPort0::ReadAsyncState::WAITING_RECEIVE) {
    it->readAsyncPromise->data = UDR0;
    it->readAsyncPromise->complete();
    it->readAsyncState = SerialPort0::ReadAsyncState::DISCONNECTED;
  }
  SerialPort0::disableReceiveInterrupt();
  SerialPort0::disableReceiver();
}


#endif //AVR_SERIALPORT0_H
