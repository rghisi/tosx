include(FetchContent)
FetchContent_Declare(libopencm3
        GIT_REPOSITORY https://github.com/rghisi/libopencm3
        GIT_TAG add-lpuart1-to-stm32g4
)
FetchContent_MakeAvailable(libopencm3)
FetchContent_GetProperties(libopencm3)