//
// Created by ghisi on 18.11.23.
//

#ifndef AVR_CONSOLE_H
#define AVR_CONSOLE_H

#include "cstdint"
#include "../system/PromiseWithReturn.h"
#include "../comms/Serial.h"
#include "cstring"
#include "cstdarg"
#include "../system/Task.h"
#include "../system/StaticStack.h"

class Console: public Task {
public:
    Console(Serial *port);
    void moveTo(uint_fast8_t row, uint_fast8_t column);
    void write(char c);
    void write(const char *string);
    void writeLine(const char *string);
    PromiseWithReturn<char> *readCharAsync();
private:
    static constexpr uint_fast8_t MAX_ROW = 24;
    static constexpr uint_fast8_t MAX_COLUMN = 80;
    uint_fast8_t row = 0, column = 0;
    Serial *port;
};

Console::Console(Serial *port): Task(new StaticStack<96>()) {
    this->port = port;
}

void Console::moveTo(uint_fast8_t row, uint_fast8_t column) {
    this->row = row;
    this->column = column;
}

void Console::write(char c) {
    port->send(c);
}

void Console::write(const char *string) {
    port->send(string, strlen(string));
}

void Console::writeLine(const char *string) {
    port->send(string, strlen(string));
    port->send("\r\n", 2);
}

PromiseWithReturn<char> *Console::readCharAsync() {
    auto promise = await(port->readCharAsync());
    return reinterpret_cast<PromiseWithReturn<char> *>(promise);
}

#endif //AVR_CONSOLE_H
