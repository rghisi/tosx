include(FetchContent)
FetchContent_Declare(libopencm3
        GIT_REPOSITORY https://github.com/libopencm3/libopencm3
        GIT_TAG master
)
FetchContent_MakeAvailable(libopencm3)
FetchContent_GetProperties(libopencm3)

# create a target to build libopencm3 -- only for the target we need
add_custom_target(libopencm3 make TARGETS=\"lm3s lm4f\" WORKING_DIRECTORY ${libopencm3_SOURCE_DIR})

# Create a specific CPU target with the appropriate options etc
add_library(ocm3 STATIC IMPORTED)
set_property(TARGET ocm3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET ocm3 PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_lm4f.a)
add_dependencies(ocm3 libopencm3)
target_link_directories(ocm3 INTERFACE ${libopencm3_SOURCE_DIR}/lib)

target_compile_definitions(ocm3 INTERFACE -DLM4F)

set(COMPILE_OPTIONS
        --static
        -nostartfiles
        -fno-common
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
)
target_compile_options(ocm3 INTERFACE ${COMPILE_OPTIONS})
target_link_options(ocm3 INTERFACE ${COMPILE_OPTIONS})


function(lm4f_add_flash_targets TARGET)
    add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Obinary ${TARGET}.elf ${TARGET}.bin
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_custom_command(
            POST_BUILD
            TARGET ${TARGET}
            COMMAND ${CMAKE_SIZE} ${TARGET}.elf
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_custom_target(${TARGET}-flash
            lm4flash ${TARGET}.bin
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${TARGET}
            VERBATIM
    )

    add_custom_target(${TARGET}-stlink-flash
            bash -c "openocd -f /usr/share/openocd/scripts/interface/ti-icdi.cfg \
                -f /usr/share/openocd/scripts/target/stellaris.cfg \
                -c 'reset_config none; program ${TARGET}.elf verify reset exit'"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${TARGET}
            VERBATIM
    )
endfunction()