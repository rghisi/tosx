//
// Created by ghisi on 1/9/23.
//

#include "comms/Serial.h"
#include "cstdio"
#include "cstring"

Serial::Serial(USART *usart) {
    this->usart = usart;
}

void Serial::send(const char *text) {
    size_t i = 0;
    while (text[i] != 0) {
        Serial::self->usart->send(text[i]);
        i++;
    }
}

void Serial::send(const uint8_t *bytes, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Serial::self->usart->send(bytes[i]);
    }
}

Promise *Serial::sendAsync(uint8_t *bytes, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Serial::self->usart->send(bytes[i]);
    }
    auto promise = new Promise();
    promise->complete();
    return promise;
}

void Serial::send(size_t num) {
    auto buffer = new char[10];
    sprintf(buffer, "%u", num);
    auto len = strlen(buffer);
    for (size_t i = 0; i < len; i++) {
        Serial::self->usart->send(buffer[i]);
    }
    delete[] buffer;
}

PromiseWithReturn<char> *Serial::readCharAsync() {
    auto promise = new PromiseWithReturn<char>();
    promise->data = 0;
    self->usart->readCharAsync(promise);
    return promise;
}

void Serial::send(char c) {
    Serial::self->usart->send(c);
}
