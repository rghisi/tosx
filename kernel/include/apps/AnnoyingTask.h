//
// Created by ghisi on 07.11.23.
//

#ifndef AVR_ANNOYINGTASK_H
#define AVR_ANNOYINGTASK_H

class AnnoyingTask: public Task {
public:
    AnnoyingTask(): Task(&staticStack) {

    }

    void run() override {
        while(true) {
            auto stringBuffer = new char[1];
            sprintf_P(stringBuffer, PSTR("."));
            auto p = await(Serial::sendAsync(stringBuffer, strlen(stringBuffer)));
            delete p;
        }
    }
private:
    StaticStack<64> staticStack = StaticStack<64>();
};

#endif //AVR_ANNOYINGTASK_H
