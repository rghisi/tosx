set(TARGET "lm4f")

include("${CMAKE_CURRENT_LIST_DIR}/cmake/toolchain-arm-none-eabi-gcc.cmake" )
include("${CMAKE_CURRENT_LIST_DIR}/arm.cmake")

add_executable(${TARGET}
        ${CMAKE_CURRENT_LIST_DIR}/lm4f/Main.cpp
)

target_link_libraries(${TARGET} ocm3)
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)
arm_add_linker_script(${TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lm4f/lm4f.ld)
lm4f_add_flash_targets(${TARGET})