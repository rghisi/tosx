//
// Created by ghisi on 03.11.23.
//

#ifndef AVR_PITASK_H
#define AVR_PITASK_H

#include "comms/Serial.h"
#include "cstdio"

class PiTask {
public:
    void calculatePi();
    static int_fast8_t run(char* args);
private:
    double bestPi = 0;
    double newPi = 0;
    double n = 0;
    double sign = 0;
    uint_fast8_t iterations = 0;
};

void PiTask::calculatePi() {
    Serial::send("PI is ");

//    bestPi = 0;
//    newPi = 3.0;
//    n = 2;
//    sign = 1.0;
//    do {
//        bestPi = newPi;
//        newPi += sign * (4.0 / ((n) * (n + 1) * (n + 2)));
//        sign = sign * (-1.0);
//        n += 2;
//        iterations++;
//    } while (std::abs(newPi - bestPi) > 0.000001);

//    auto stringBuffer = new char[18];
//    sprintf(stringBuffer, "%f (%d)\n\r", bestPi, iterations);
//    Serial::send(stringBuffer);

    Serial::send("PI Iterations=");
    Serial::send(iterations);
    Serial::send("\n\r");

//    delete[] stringBuffer;
}

int_fast8_t PiTask::run(char *args) {
    auto piTask = new PiTask();
    piTask->calculatePi();
    delete piTask;

    return 0;
}

#endif //AVR_PITASK_H
