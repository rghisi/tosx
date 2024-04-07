//
// Created by ghisi on 19.11.23.
//

#ifndef AVR_KEYBOARD_H
#define AVR_KEYBOARD_H

#include "KeyPressEvent.h"
#include "../system/OS.h"

class Keyboard {
public:
    void handleKeyPress(char key);
};

void Keyboard::handleKeyPress(char key) {
    auto event = new KeyPressEvent(key);
    OS::dispatch(event);
}

#endif //AVR_KEYBOARD_H
