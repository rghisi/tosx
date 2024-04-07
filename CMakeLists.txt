cmake_minimum_required(VERSION 3.23)
project(tosx CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_CXX_FLAGS_DEBUG "-Og")

#ATMega328p (Arduino) - 2K RAM 32K FLASH
#include(avr/atmega328p.cmake)

#ATMega32u4 - 2.5K RAM 32K FLASH
include(avr/atmega32u4.cmake)

#Kernel
include(kernel/CMakeLists.txt)
