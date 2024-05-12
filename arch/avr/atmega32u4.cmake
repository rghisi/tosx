set(TARGET_CPU atmega32u4)
set(F_CPU 16000000L)
set(TARGET "atmega32u4")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-avr-gcc.cmake")

add_executable(${TARGET}
        ${CMAKE_CURRENT_LIST_DIR}/atmega32u4/Main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/atmega32u4/SerialPort0.h
)

add_custom_target(${TARGET}-flash
        ${AVR_DUDE_EXECUTABLE} -p ${TARGET_CPU} -cavr109 -b57600 -P ${AVR_DUDE_PORT} -v -F -D -U flash:w:${PROJECT_BINARY_DIR}/${TARGET}.elf:a
        DEPENDS ${TARGET}
        COMMENT "Flash to ${TARGET_CPU}")

add_custom_command(
        POST_BUILD
        TARGET ${TARGET}
        COMMAND ${CMAKE_SIZE} --mcu=${TARGET_CPU} -C ${TARGET}.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
