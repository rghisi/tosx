#include "C.h"
#include "Cpp.h"
#include "Lm4fCpu.h"
#include "SerialPort0.h"
#include "comms/Serial.h"
#include "shell/Shell.h"
#include "std/Random.h"
#include "std/SoftwareRandomProvider.h"
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

auto ma = DoublyLinkedMemoryAllocator<28672>();

MemoryAllocator *OS::memoryAllocator = &ma;
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);

Serial *Serial::self = &serial;
auto cpu = Lm4fCpu();

int main() {
  setbuf(stdout, NULL);
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
