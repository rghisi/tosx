cmake_minimum_required(VERSION 3.22)
project(tosx CXX C)
set(CMAKE_CXX_STANDARD 20)

add_compile_options("-I${CMAKE_SOURCE_DIR}/lib/include")
add_compile_options("-I${CMAKE_SOURCE_DIR}/kernel/include")
add_compile_options("-I${CMAKE_SOURCE_DIR}/apps/include")

if (NOT CMAKE_BUILD_TYPE STREQUAL "Tests")
    #ATMega328p (Arduino) - 2K RAM 32K FLASH
#    include(arch/avr/atmega328p.cmake)

    #ATMega32u4 - 2.5K RAM 32K FLASH
    #include(arch/avr/atmega32u4.cmake)

    #TI LM4F series
#    include(arch/arm/lm4f.cmake)

    #STM32G474
    include(arch/arm/stm32g474.cmake)

    include(kernel/CMakeLists.txt)
    include(apps/CMakeLists.txt)
    include(lib/CMakeLists.txt)

else()
    set(CMAKE_CXX_FLAGS_TESTS "-g")
    include(tests/CMakeLists.txt)

    include(kernel/CMakeLists.txt)
    include(apps/CMakeLists.txt)
    include(lib/CMakeLists.txt)

    include(kernel/tests/CMakeLists.txt)
    include(lib/tests/CMakeLists.txt)
endif ()


