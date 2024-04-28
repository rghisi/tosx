//
// Created by ghisi on 1/9/23.
//

#include "comms/Serial.h"

#include "cstdio"

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

static char IntToHex(size_t x) {
  x &= 15;
  if (x <= 9) return x + '0';
  return x - 10 + 'A';
}

void Serial::send(size_t num) {
  Serial::self->usart->send('0');
  Serial::self->usart->send('x');
  Serial::self->usart->send(IntToHex(num >> 28));
  Serial::self->usart->send(IntToHex(num >> 24));
  Serial::self->usart->send(IntToHex(num >> 20));
  Serial::self->usart->send(IntToHex(num >> 16));
  Serial::self->usart->send(IntToHex(num >> 12));
  Serial::self->usart->send(IntToHex(num >> 8));
  Serial::self->usart->send(IntToHex(num >> 4));
  Serial::self->usart->send(IntToHex(num));
  //  auto buffer = new char[12];
  //  itoa(num, buffer, 10);
  //  auto len = strlen(buffer);
  //  for (size_t i = 0; i < len; i++) {
  //      Serial::self->usart->send(buffer[i]);
  //  }
  //  delete[] buffer;

  //    auto buffer = new char[24];
  //    sprintf(buffer, "%d", num);
  //    size_t i = 0;
  //    while (buffer[i] != 0) {
  //        Serial::self->usart->send(buffer[i]);
  //        i++;
  //    }
  //    delete[] buffer;
}

PromiseWithReturn<char> *Serial::readCharAsync() {
  auto promise = new PromiseWithReturn<char>();
  promise->data = 0;
  self->usart->readCharAsync(promise);
  return promise;
}

void Serial::send(char c) { Serial::self->usart->send(c); }
