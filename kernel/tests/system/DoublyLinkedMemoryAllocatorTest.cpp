//
// Created by ghisi on 17.09.22.
//

#include <gtest/gtest.h>
#include "system/DoublyLinkedMemoryAllocator.h"

TEST(MemoryAllocator, ShouldHave100PercentFreeWhenStarting) {
    constexpr auto managedMemorySize = 640;
    constexpr auto blockOverhead = DoublyLinkedMemoryAllocator<managedMemorySize>::AllocationOverhead;
    auto allocator = DoublyLinkedMemoryAllocator<managedMemorySize>();

    auto expectedAvailableMemory = managedMemorySize - blockOverhead;

//    ASSERT_EQ(expectedAvailableMemory, allocator.memoryAvailable());
}

TEST(MemoryAllocator, ShouldConsumeMemoryAllocatedPlusOneBlockWhenAllocatingMemory) {
    constexpr auto managedMemorySize = 640;
    auto allocator = DoublyLinkedMemoryAllocator<managedMemorySize>();

    auto sOne = sizeof(unsigned int[10]);
    auto *allocationAddressOne = allocator.allocate(sOne);
    auto *dataOne = new (allocationAddressOne) unsigned int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto sTwo = sizeof(unsigned int[7]);
    auto *allocationAddressTwo = allocator.allocate(sTwo);
    auto *dataTwo = new (allocationAddressTwo) unsigned int[7]{11, 12, 13, 14, 15, 16, 17};

    ASSERT_NE(nullptr, allocationAddressOne);
    ASSERT_EQ(1, dataOne[0]);
    ASSERT_EQ(2, dataOne[1]);
    ASSERT_EQ(3, dataOne[2]);
    ASSERT_EQ(4, dataOne[3]);
    ASSERT_EQ(5, dataOne[4]);
    ASSERT_EQ(6, dataOne[5]);
    ASSERT_EQ(7, dataOne[6]);
    ASSERT_EQ(8, dataOne[7]);
    ASSERT_EQ(9, dataOne[8]);
    ASSERT_EQ(10, dataOne[9]);

    ASSERT_NE(nullptr, allocationAddressTwo);
    ASSERT_EQ(11, dataTwo[0]);
    ASSERT_EQ(12, dataTwo[1]);
    ASSERT_EQ(13, dataTwo[2]);
    ASSERT_EQ(14, dataTwo[3]);
    ASSERT_EQ(15, dataTwo[4]);
    ASSERT_EQ(16, dataTwo[5]);
    ASSERT_EQ(17, dataTwo[6]);
}

TEST(MemoryAllocator, ShouldFreeMemoryAllocatedAndMergeAllocations) {
    constexpr auto managedMemorySize = 640;
    auto allocator = DoublyLinkedMemoryAllocator<managedMemorySize>();

    auto sOne = sizeof(unsigned int[10]);
    auto *allocationAddressOne = allocator.allocate(sOne);
    auto *dataOne = new (allocationAddressOne) unsigned int[10]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto sTwo = sizeof(unsigned int[7]);
    auto *allocationAddressTwo = allocator.allocate(sTwo);
    auto *dataTwo = new (allocationAddressTwo) unsigned int[7]{11, 12, 13, 14, 15, 16, 17};

    allocator.free(allocationAddressOne);
    allocator.free(allocationAddressTwo);
}
