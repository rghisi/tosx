set(TARGET_CPU atmega328p)
set(F_CPU 16000000L)
set(TARGET "atmega328p")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-avr-gcc.cmake")

add_executable(${TARGET}
        ${CMAKE_CURRENT_LIST_DIR}/atmega328p/Main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/atmega328p/SerialPort0.h
)

add_custom_target(${TARGET}-flash
        ${AVR_DUDE_EXECUTABLE} -p ${TARGET_CPU} -carduino -b115200 -P ${AVR_DUDE_PORT} ${FLASH_VERBOSE_FLAG} -F -D -U flash:w:${TARGET}.elf:a
        DEPENDS ${TARGET}
        COMMENT "Flash to ${TARGET_CPU}")

add_custom_command(
        POST_BUILD
        TARGET ${TARGET}
        COMMAND ${CMAKE_SIZE} --mcu=${TARGET_CPU} -C ${TARGET}.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)