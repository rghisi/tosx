# TOSX - The OS eXperiment

Welcome to The OS eXperiment project, an initiative to research OS development starting simple and complicating
later.

This OS is a good candidate to be the foundation for a future homebrew computer.

## Goals
* Multiplatform support - reached when running in two different archs. e.g. ARM + AVR
  * AVR 8bit microcontrollers
    * [x] ATMega328p - 2KB RAM + 32KB FLASH
      * enough for initial experimentation, small programs and up to two or three simultaneous apps running
    * [x] ATMega32u4 - 2.5KB RAM + 32KB FLASH
      * a bit more headroom for running apps
    * [ ] ATMega2560 - 8KB RAM + 256KB FLASH
      * starts to get interesting?
  * ARM CortexM4
    * [ ] STM32G431
    * [ ] TI LM4F120 (because it is available)
  * x86_32
    * [ ] QEMU
  * M68K
    * [ ] Sega Mega Drive/Genesis - future project
* Multitasking
  * [x] Cooperative (might still be possible)
  * [x] Preemptive (now preferred)
* [x] Basic Async/Await mechanism aimed for I/O
* Shell
  * [x] Start new processes (blocking)
  * [x] Start new processes (background)
  * [x] Command line arguments to downstream processes
  * [ ] Better usability
  * [ ] More...
* Memory Management
  * [x] Simple Allocator
    * Doubly-linked - a bit high on overhead
    * Merging of free blocks to reduce fragmentation
    * Best-fit search to reduce fragmentation
* [x] Static stack allocation for tasks
* [x] Heap stack allocation for tasks
* Sample apps
  * `clear` - clear screen 
  * `free` - show memory stats
  * `pi` - calculates PI to 6digits
  * `long` - runs a long sleep (10s) to exercise async/await and timing
  * `cdn` - runs a countdown from 10, can be used to exercise multitasking
  * `uptime` - shows uptime since last reset (ugly output formatting)
* [ ] Filesystem
* [ ] File support