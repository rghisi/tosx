set(TARGET "tosxtests")

add_executable(${TARGET}
        ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
)

enable_testing()

target_link_libraries(${TARGET} gtest)
target_link_libraries(${TARGET} gmock)
