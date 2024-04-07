set(TARGET_CPU atmega32u4)
set(F_CPU 16000000L)

set(AVR_DUDE_EXECUTABLE /usr/bin/avrdude)
set(AVR_DUDE_PORT /dev/ttyACM0)

set(CMAKE_C_COMPILER "/opt/avr-gcc/bin/avr-gcc")
set(CMAKE_CXX_COMPILER "/opt/avr-gcc/bin/avr-g++")

add_compile_definitions("F_CPU=${F_CPU}")
add_compile_options("-mmcu=${TARGET_CPU}")
add_compile_options("-mcall-prologues")
add_compile_options("-I${CMAKE_SOURCE_DIR}/avr/avr-libstdcpp/include")
add_compile_options("-I${CMAKE_SOURCE_DIR}/kernel/include")
add_compile_options("-fdata-sections")
add_compile_options("-ffunction-sections")
add_compile_options("-finline-functions")
#add_compile_options("-mdouble=64")
#add_compile_options("-fpermissive")

add_link_options(-Wl,--print-memory-usage,--gc-sections,-u,vfprintf -lprintf_flt -lm)
add_link_options("-mmcu=${TARGET_CPU}")

add_executable(${PROJECT_NAME}
        avr/generic/AVRContextSwitching.h
        avr/generic/AVRCpu.h
        avr/generic/AVRCpu.cpp
        avr/atmega32u4/SerialPort0.h
        avr/atmega32u4/SerialPort0.cpp
        avr/atmega32u4/Main.cpp
)

set(FLASH_VERBOSE_FLAG "-v")

add_custom_target(FLASH
        ${AVR_DUDE_EXECUTABLE} -p ${TARGET_CPU} -cavr109 -b57600 -P ${AVR_DUDE_PORT} ${FLASH_VERBOSE_FLAG} -F -D -U flash:w:${PROJECT_BINARY_DIR}/${PROJECT_NAME}:a
        DEPENDS ${PROJECT_NAME}
        COMMENT "Flash to ${TARGET_CPU}")

add_custom_command(
        POST_BUILD
        TARGET ${PROJECT_NAME}
        COMMAND /opt/avr-gcc/bin/avr-size --mcu=atmega32u4 -C ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
        COMMENT "Size:"
)
