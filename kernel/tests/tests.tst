cmake_minimum_required(VERSION 3.14)
project(AVROSTests CXX C)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS_DEBUG "-Os")

add_executable(${PROJECT_NAME}
        src/Main.cpp
        ../../avr/src/collections/BlockingQueue.h src/collections/BlockingQueueTest.cpp
        ../../avr/src/collections/Array.h src/collections/ArrayTest.cpp
        ../../avr/src/collections/PriorityQueue.h ../../avr/src/collections/PriorityQueue.cpp src/collections/PriorityQueueTest.cpp
        src/system/DoublyLinkedMemoryAllocatorTest.cpp ../../avr/src/system/DoublyLinkedMemoryAllocator.h
        src/collections/StaticPriorityQueueTest.cpp ../../avr/src/collections/StaticPriorityQueue.h
        src/collections/FragmentedTextTest.cpp ../../avr/src/collections/FragmentedText.h
        src/std/StringTest.cpp ../avr/src/std/String.h
        src/console/CommandLineTest.cpp
        src/collections/LinkedListTest.cpp ../../avr/src/collections/LinkedList.h
)

enable_testing()

target_link_libraries(${PROJECT_NAME} gtest)
target_link_libraries(${PROJECT_NAME} gmock)