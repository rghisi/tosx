set(TARGET "stm32g474")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-arm-none-eabi-gcc.cmake" )
include("${CMAKE_CURRENT_LIST_DIR}/libopencm3.cmake")

add_executable(${TARGET}
        ${CMAKE_CURRENT_LIST_DIR}/stm32g474/Main.cpp
)

# create a target to build libopencm3 -- only for the target we need
add_custom_target(libopencm3 make TARGETS=stm32/g4 WORKING_DIRECTORY ${libopencm3_SOURCE_DIR})

# Create a specific CPU target with the appropriate options etc
add_library(libopencm3_stm32g4 STATIC IMPORTED)
set_property(TARGET libopencm3_stm32g4 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET libopencm3_stm32g4 PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_stm32g4.a)
add_dependencies(libopencm3_stm32g4 libopencm3)
target_link_directories(libopencm3_stm32g4 INTERFACE ${libopencm3_SOURCE_DIR}/lib)

target_compile_definitions(libopencm3_stm32g4 INTERFACE -DSTM32G4)

set(COMPILE_OPTIONS
        --static
        -nostartfiles
        -fno-common
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)
target_compile_options(libopencm3_stm32g4 INTERFACE ${COMPILE_OPTIONS})
target_link_options(libopencm3_stm32g4 INTERFACE ${COMPILE_OPTIONS})

target_link_libraries(${TARGET} libopencm3_stm32g4)
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 20)
arm_add_linker_script(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/stm32g474/stm32g474.ld)

add_custom_command(
        POST_BUILD
        TARGET ${TARGET}
        COMMAND ${CMAKE_SIZE} ${TARGET}.elf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_target(${TARGET}-stlink-flash
        bash -c "/opt/openocd/bin/openocd -f /opt/openocd/openocd/scripts/board/st_nucleo_g4.cfg \
                -c 'program ${TARGET}.elf verify reset exit'"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS ${TARGET}
        VERBATIM
)
