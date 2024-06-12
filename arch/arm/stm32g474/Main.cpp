#include <cstdio>
#include "C.h"
#include "Cpp.h"
#include "SerialPort0.h"
#include "Stm32RandomProvider.h"
#include "Stm32g474Cpu.h"
#include "comms/Serial.h"
#include "std/Random.h"
#include "system/DoublyLinkedMemoryAllocator.h"
#include "system/OS.h"
#include "misc/Init.h"

extern "C" int _write(int handle, char *data, int size )
{
  int count;
  for(count = 0; count < size; count++) {
    Serial::send(data[count]);
  }

  return count;
}

auto ma = DoublyLinkedMemoryAllocator<129024>();
auto serialPort0 = SerialPort0();
auto serial = Serial(&serialPort0);

auto cpu = Stm32g474();

int main() {
  OS::memoryAllocator = &ma;
  setbuf(stdout, NULL);
  cpu.setup();
  serialPort0.setup();
  Serial::self = &serial;
  Random::setup(new Stm32RandomProvider());
  auto scheduler = Scheduler();
  auto kernel = Kernel(&cpu, &scheduler);
  OS::kernel = &kernel;
  kernel.schedule(OS::createTask("Init", Init::run, nullptr));
  kernel.start();

  return 0;
}
