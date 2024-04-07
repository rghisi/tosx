//
// Created by ghisi on 05.11.23.
//

#ifndef AVR_PROMISE_H
#define AVR_PROMISE_H

class Promise {
public:
    Promise() = default;
    virtual ~Promise() = default;

    [[nodiscard]] virtual bool isCompleted() {
        return completed;
    }
    void complete() {
        completed = true;
    }

protected:
    bool completed = false;
};
#endif //AVR_PROMISE_H
