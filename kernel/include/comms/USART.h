//
// Created by ghisi on 05.09.22.
//

#ifndef AVR_USART_H
#define AVR_USART_H

#include "cstdint"
#include "../system/PromiseWithReturn.h"

class USART {
public:
    virtual void disableTransmitter() = 0;
    virtual void enableTransmitter() = 0;
    virtual void send(uint8_t byte) = 0;
    virtual void readCharAsync(PromiseWithReturn<char> *promise) = 0;
};

#endif //AVR_USART_H
