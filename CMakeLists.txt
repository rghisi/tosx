cmake_minimum_required(VERSION 3.23)
project(tosx CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_CXX_FLAGS_DEBUG "-Og")

add_compile_options("-I${CMAKE_SOURCE_DIR}/lib/include")
add_compile_options("-I${CMAKE_SOURCE_DIR}/kernel/include")
add_compile_options("-I${CMAKE_SOURCE_DIR}/apps/include")

if (NOT CMAKE_BUILD_TYPE STREQUAL "Tests")
    #ATMega328p (Arduino) - 2K RAM 32K FLASH
    #include(avr/atmega328p.cmake)

    #ATMega32u4 - 2.5K RAM 32K FLASH
    include(avr/atmega32u4.cmake)


    include(kernel/CMakeLists.txt)
    include(apps/CMakeLists.txt)
else()
    set(CMAKE_CXX_FLAGS_TESTS "-g")
    include(tests/CMakeLists.txt)
    include(kernel/tests/CMakeLists.txt)
    include(lib/tests/CMakeLists.txt)
endif ()

include(lib/CMakeLists.txt)
