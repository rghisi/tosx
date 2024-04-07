//
// Created by ghisi on 04.09.22.
//

#include "CRC.h"
#include "CRCTable.c"

CRC::CRC() {
    crc = START_CRC;
}

void CRC::add(uint8_t byte) {
    uint8_t data = byte ^ crc;
    crc = pgm_read_byte(&(CRCTable[data])) ^ crc << 8;
}

uint8_t CRC::value() {
    return crc;
}

uint8_t CRC::calculate(const uint8_t *message, uint8_t nBytes) {
    uint8_t crc = START_CRC;
    uint8_t data;
    for (uint8_t byte = 0; byte < nBytes; ++byte) {
        data = message[byte] ^ crc;
        crc = pgm_read_byte(&(CRCTable[data])) ^ crc << 8;
    }

    return crc;
}


