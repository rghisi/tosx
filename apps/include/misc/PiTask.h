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
    uint32_t iterations = 0;
};

void PiTask::calculatePi() {
    printf("PI is...");
    auto now = OS::now();
    bestPi = 0;
    newPi = 3.0;
    n = 2;
    sign = 1.0;
    do {
        bestPi = newPi;
        newPi += sign * (4.0 / ((n) * (n + 1) * (n + 2)));
        sign = sign * (-1.0);
        n += 2;
        iterations++;
    } while (std::abs(newPi - bestPi) > 0.000001);
    auto elapsed = OS::now() - now;
    printf("%lums (%lu)\r\n", elapsed, iterations);
}

int_fast8_t PiTask::run(char *args) {
    auto piTask = new PiTask();
    piTask->calculatePi();
    delete piTask;

    return 0;
}

#endif //AVR_PITASK_H
