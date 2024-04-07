#include "cstdint"
#include "setup/C.h"
#include "setup/Cpp.h"
#include "../generic/AVRCpu.h"
#include "SerialPort0.h"
#include "comms/Serial.h"
#include "system/DoublyLinkedMemoryAllocator.h"
#include "system/OS.h"
#include "console/Shell.h"

Kernel* OS::kernel = nullptr;
auto ma = DoublyLinkedMemoryAllocator<1280>();

MemoryAllocator *OS::memoryAllocator = &ma;
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);

Serial *Serial::self = &serial;

int main() {
    auto scheduler = new Scheduler();
    auto cpu = new AVRCpu();
    scheduler->schedule(OS::createTask(Shell::run, nullptr));
    auto kernel = new Kernel(
            cpu,
            scheduler
        );
    OS::kernel = kernel;
    kernel->start();

    return 0;
}
