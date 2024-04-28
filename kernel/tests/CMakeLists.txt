add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/system/DoublyLinkedMemoryAllocatorTest.cpp
)

enable_testing()

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)

