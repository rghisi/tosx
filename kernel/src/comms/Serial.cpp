//
// Created by ghisi on 1/9/23.
//

#include "comms/Serial.h"

#include "cstdio"

Serial *Serial::self = nullptr;

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

void Serial::send(uint32_t value) {
    char buffer[11];
    char *p = &buffer[10];
    *p = '\0';
    do {
        *--p = "0123456789"[value % 10];
    } while (value /= 10);
    send(p);
}

void Serial::send(size_t num) {
  send((uint32_t) num);
}

PromiseWithReturn<char> *Serial::readCharAsync() {
  return self->usart->readCharAsync();
}

void Serial::send(char c) {
  Serial::self->usart->send(c);
}
