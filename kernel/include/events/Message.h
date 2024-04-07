//
// Created by ghisi on 09.10.22.
//

#ifndef AVR_MESSAGE_H
#define AVR_MESSAGE_H

#include "MessageType.h"
#include "memory"

class Message {
public:
    explicit Message(MessageType type);
    virtual ~Message();
    MessageType type();
private:
    MessageType eType;
};

Message::Message(MessageType type) {
    this->eType = type;
}

Message::~Message() = default;

MessageType Message::type() {
    return eType;
}

#endif //AVR_MESSAGE_H
