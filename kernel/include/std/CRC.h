//
// Created by ghisi on 04.09.22.
//

#ifndef AVR_CRC_H
#define AVR_CRC_H

#include <cstdint>

class CRC {
public:
    static uint8_t calculate(const uint8_t* message, uint8_t nBytes);
    CRC();
    void add(uint8_t byte);
    uint8_t value();
private:
    uint8_t crc;
    static const uint8_t START_CRC = 0xFD;
};


#endif //AVR_CRC_H
