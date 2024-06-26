#include "C.h"
#include "Cpp.h"
#include "generic/AVRCpu.h"
#include "SerialPort0.h"
#include "comms/Serial.h"
#include "system/DoublyLinkedMemoryAllocator.h"
#include "system/OS.h"
#include "shell/Shell.h"

Kernel* OS::kernel = nullptr;
auto ma = DoublyLinkedMemoryAllocator<1792>();

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
