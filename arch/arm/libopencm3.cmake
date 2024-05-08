include(FetchContent)
FetchContent_Declare(libopencm3
        GIT_REPOSITORY https://github.com/libopencm3/libopencm3
        GIT_TAG master
)
FetchContent_MakeAvailable(libopencm3)
FetchContent_GetProperties(libopencm3)