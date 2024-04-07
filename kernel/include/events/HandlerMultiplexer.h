//
// Created by ghisi on 1/2/23.
//

#ifndef AVR_HANDLERMULTIPLEXER_H
#define AVR_HANDLERMULTIPLEXER_H

#include "MessageType.h"
#include "Subscriber.h"
#include "list"

class HandlerMultiplexer: public Subscriber {
public:
    ~HandlerMultiplexer();
    void handle(Message* message) override;
    void add(Subscriber *handler);
private:
    std::list<Subscriber*> handlers;
};

HandlerMultiplexer::~HandlerMultiplexer() = default;

void HandlerMultiplexer::handle(Message *message) {
    for (auto handler: handlers) {
        handler->handle(message);
    }
}

void HandlerMultiplexer::add(Subscriber *handler) {
    this->handlers.push_back(handler);
}

#endif //AVR_HANDLERMULTIPLEXER_H
