#include "C.h"
#include "Cpp.h"
#include "SerialPort0.h"
#include "comms/Serial.h"
#include "cstdio"
#include "generic/AVRCpu.h"
#include "shell/Shell.h"
#include "std/Random.h"
#include "std/SoftwareRandomProvider.h"
#include "system/DoublyLinkedMemoryAllocator.h"
#include "system/OS.h"

#undef FDEV_SETUP_STREAM
#define FDEV_SETUP_STREAM(PU, GE, FL) \
    {                                 \
        0  /* buf */,                 \
        0  /* unget */,               \
        FL /* flags */,               \
        0  /* size */,                \
        0  /* len */,                 \
        PU /* put */,                 \
        GE /* get */,                 \
        0  /* udata */                \
    }


extern "C" {
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
}

int uart_putchar(char c, FILE *stream) {
  Serial::send(c);
  return 0;
}

auto ma = DoublyLinkedMemoryAllocator<512>();

MemoryAllocator *OS::memoryAllocator = &ma;
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);

Serial *Serial::self = &serial;
auto cpu = AVRCpu();

int main() {
  stdout = &mystdout;
  cpu.setup();
  serialPort0.setup();
  Random::setup(new SoftwareRandomProvider());
  auto scheduler = Scheduler();
  auto kernel = Kernel(&cpu, &scheduler);
  OS::kernel = &kernel;
  kernel.schedule(OS::createTask("shell", Shell::run, nullptr));
  kernel.start();

  return 0;
}
