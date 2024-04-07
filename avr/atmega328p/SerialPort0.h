//
// Created by ghisi on 09.11.22.
//

#ifndef AVR_SERIALPORT0_H
#define AVR_SERIALPORT0_H

#include "cstdint"
#include "comms/USART.h"
#include "vector"

extern "C" void USART_RX_vect(void) __attribute__ ((signal));

class SerialPort0: public USART {
public:
    SerialPort0();
    void send(uint8_t byte) override;
    void readCharAsync(PromiseWithReturn<char> *promise) override;
    void disableTransmitter() override;
    void enableTransmitter() override;
    friend void USART_RX_vect(void);

private:
    static SerialPort0 *it;
    enum class ReadAsyncState: uint_fast8_t { DISCONNECTED, WAITING_RECEIVE };
    ReadAsyncState readAsyncState = ReadAsyncState::DISCONNECTED;
    PromiseWithReturn<char> *readAsyncPromise = nullptr;
    static void disableReceiver();
    static void enableReceiver();
    static void enableReceiveInterrupt();
    static void disableReceiveInterrupt();
};


#endif //AVR_SERIALPORT0_H
