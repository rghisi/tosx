#include "C.h"
#include "Cpp.h"
#include "SerialPort0.h"
#include "comms/Serial.h"
#include "generic/CortexM4Cpu.h"
#include "shell/Shell.h"
#include "system/DoublyLinkedMemoryAllocator.h"
#include "system/OS.h"

Kernel* OS::kernel = nullptr;
auto ma = DoublyLinkedMemoryAllocator<28672>();

MemoryAllocator *OS::memoryAllocator = &ma;
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);

Serial *Serial::self = &serial;

int main() {
  auto cpu = new CortexM4Cpu();
  cpu->setup();
  serialPort0.setup();
  Serial::send('1');
  auto scheduler = new Scheduler();
  Serial::send('2');
  auto kernel = new Kernel(
      cpu,
      scheduler
  );
  Serial::send('3');
  kernel->schedule(OS::createTask("shell", Shell::run, nullptr));
  OS::kernel = kernel;
  kernel->start();

  return 0;
}
