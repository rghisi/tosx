//
// Created by ghisi on 09.11.22.
//

#include <avr/interrupt.h>
#include "SerialPort0.h"

SerialPort0 *SerialPort0::it = nullptr;

SerialPort0::SerialPort0() {
    UCSR1C = (_BV(UCSZ11)) | (_BV(UCSZ10));
    UBRR1 = 0; //1mbps
    SerialPort0::it = this;
    SerialPort0::it->enableTransmitter();
}

void SerialPort0::disableReceiver() {
    UCSR1B &= ~(_BV(RXEN1));
}

void SerialPort0::enableReceiver() {
    UCSR1B |= _BV(RXEN1);
}

void SerialPort0::disableTransmitter() {
    UCSR1B &= ~(_BV(TXEN1));
}

void SerialPort0::enableTransmitter() {
    UCSR1B |= _BV(TXEN1);
}

void SerialPort0::send(uint8_t byte) {
    while (!(UCSR1A & (1<<UDRE1)));
    UDR1 = byte;
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
    UCSR1B |= _BV(RXCIE1);
}

void SerialPort0::disableReceiveInterrupt() {
    UCSR1B &= ~(_BV(RXCIE1));
}

void USART1_RX_vect(void) {
    auto it = SerialPort0::it;
    if (it != nullptr && it->readAsyncState == SerialPort0::ReadAsyncState::WAITING_RECEIVE) {
        it->readAsyncPromise->data = UDR1;
        it->readAsyncPromise->complete();
        it->readAsyncState = SerialPort0::ReadAsyncState::DISCONNECTED;
    }
    SerialPort0::disableReceiveInterrupt();
    SerialPort0::disableReceiver();
}