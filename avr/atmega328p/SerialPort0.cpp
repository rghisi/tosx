//
// Created by ghisi on 09.11.22.
//

#include <avr/interrupt.h>
#include "SerialPort0.h"

SerialPort0 *SerialPort0::it = nullptr;

SerialPort0::SerialPort0() {
    UCSR0C = (_BV(UCSZ01)) | (_BV(UCSZ00));
    UBRR0 = 0; //1mbps
    SerialPort0::it = this;
    SerialPort0::it->enableTransmitter();
}

void SerialPort0::disableReceiver() {
    UCSR0B &= ~(_BV(RXEN0));
}

void SerialPort0::enableReceiver() {
    UCSR0B |= _BV(RXEN0);
}

void SerialPort0::disableTransmitter() {
    UCSR0B &= ~(_BV(TXEN0));
}

void SerialPort0::enableTransmitter() {
    UCSR0B |= _BV(TXEN0);
}

void SerialPort0::send(uint8_t byte) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = byte;
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