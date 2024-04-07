add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/collections/CircularQueueTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/collections/FragmentedTextTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/collections/LinkedListTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/collections/PriorityQueueTest.cpp
        ${CMAKE_CURRENT_LIST_DIR}/collections/StaticPriorityQueueTest.cpp
)

enable_testing()

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)

