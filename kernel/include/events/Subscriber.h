//
// Created by ghisi on 09.10.22.
//

#ifndef AVR_SUBSCRIBER_H
#define AVR_SUBSCRIBER_H

#include "memory"
#include "MessageType.h"
#include "Message.h"

class Subscriber {
public:
    virtual void handle(Message* message) = 0;
};



#endif //AVR_SUBSCRIBER_H
