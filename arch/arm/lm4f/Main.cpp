#include "C.h"
#include "Cpp.h"
#include "SerialPort0.h"
#include "comms/Serial.h"
#include "generic/CortexM4Cpu.h"
#include "shell/Shell.h"
#include "system/DoublyLinkedMemoryAllocator.h"
#include "system/OS.h"

extern "C" int _write(int handle, char *data, int size )
{
  int count;
  for(count = 0; count < size; count++) {
    Serial::send(data[count]);
  }

  return count;
}

Kernel* OS::kernel = nullptr;
auto ma = DoublyLinkedMemoryAllocator<28672>();

MemoryAllocator *OS::memoryAllocator = &ma;
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);

Serial *Serial::self = &serial;
auto cpu = CortexM4Cpu();

int main() {
  auto scheduler = Scheduler();
  auto kernel = Kernel(&cpu, &scheduler);
  OS::kernel = &kernel;
  cpu.setup();
  serialPort0.setup();
  kernel.schedule(OS::createTask("shell", Shell::run, nullptr));
  kernel.start();

  return 0;
}
