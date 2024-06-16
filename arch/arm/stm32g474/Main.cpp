#include <cstdio>
#include "C.h"
#include "Cpp.h"
#include "SerialPort0.h"
#include "Stm32RandomProvider.h"
#include "Stm32g474Cpu.h"
#include "comms/Serial.h"
#include "std/Random.h"
#include "system/OS.h"
#include "misc/Init.h"
#include "system/SinglyLinkedMemoryAllocator.h"

static constexpr const char* CLEAR_SCREEN = "\x1b[2J\x1b[H";

extern "C" uint32_t _sbss;
extern "C" uint32_t _ebss;
extern "C" uint32_t _stack;

extern "C" int _write(int handle, char *data, int size) {
    int count;
    for (count = 0; count < size; count++) {
        Serial::send(data[count]);
    }

    return count;
}

auto cpu = Stm32g474();
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);
auto randomProvider = Stm32RandomProvider();
auto memoryAllocator = SinglyLinkedMemoryAllocator(((uint32_t) &_ebss) + 512, ((uint32_t) &_stack) - 512);
auto scheduler = Scheduler();
auto kernel = Kernel(&cpu, &scheduler, &memoryAllocator);

int main() {
    cpu.setup();
    setbuf(stdout, NULL);
    serialPort0.setup();
    Serial::self = &serial;
    Serial::send(CLEAR_SCREEN);
    printf("The OS eXperiment\r\n");
    printf("Build %s %s\r\n", __DATE__,  __TIME__);
    printf("Boot...\r\n");
    Random::setup(&randomProvider);
    OS::kernel = &kernel;
    kernel.schedule(OS::createTask("Init", Init::run, nullptr));
    kernel.start();
    printf("\r\nSystem Crashed Successfully\r\n");

    return 0;
}
