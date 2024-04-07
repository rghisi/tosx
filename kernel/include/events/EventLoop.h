//
// Created by ghisi on 09.10.22.
//

#ifndef AVR_EVENTLOOP_H
#define AVR_EVENTLOOP_H


#include "Message.h"
#include "Subscriber.h"
#include "memory"
#include "list"
#include "any"
#include "SubscriberRegistry.h"
#include "../collections/CircularQueue.h"

class EventLoop {
public:
    EventLoop(SubscriberRegistry *subscriberRegistry);
    void process();
    bool push(Message* event);
private:
    static constexpr uint8_t BUFFER_SIZE = 10;
    CircularQueue<Message*, BUFFER_SIZE> events;
    SubscriberRegistry *subscriberRegistry;
};

EventLoop::EventLoop(SubscriberRegistry *subscriberRegistry) {
    this->subscriberRegistry = subscriberRegistry;
}

void EventLoop::process() {
    if (!events.isEmpty()) {
        auto event = events.pop();
        if (event) {
            auto *subscriber = subscriberRegistry->get(event->type());
            if (subscriber != nullptr) {
                subscriber->handle(event);
            }
        }
        delete event;
    }
}

bool EventLoop::push(Message* event) {
    return events.push(event);
}

#endif //AVR_EVENTLOOP_H
