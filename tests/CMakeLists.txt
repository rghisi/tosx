include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

set(TARGET "tosxtests")

add_executable(${TARGET}
        ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Mutex.cpp
)

enable_testing()

target_link_libraries(${TARGET} gtest gmock_main)
