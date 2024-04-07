//
// Created by ghisi on 09.10.22.
//

#ifndef AVR_MESSAGETYPE_H
#define AVR_MESSAGETYPE_H

#include "cstdint"

enum MessageType: uint8_t {
    ASYNC_SCHEDULED,
    ASYNC_CHAIN_SCHEDULED,
    SERIAL_SEND,
    MAX,
};


#endif //AVR_MESSAGETYPE_H
