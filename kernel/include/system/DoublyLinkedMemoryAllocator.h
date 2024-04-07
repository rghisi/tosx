//
// Created by ghisi on 22.03.23.
//

#ifndef AVR_DOUBLYLINKEDMEMORYALLOCATOR_H
#define AVR_DOUBLYLINKEDMEMORYALLOCATOR_H


#include "cstdint"
#include "cstddef"
#include "MemoryAllocator.h"

enum class AllocationFlags { FREE = 0x00, USED = 0xFF };

class Allocation {
public:
    explicit Allocation(size_t size) {
        this->size = size + sizeof(Allocation);
        flags = AllocationFlags::FREE;
    }
    size_t size;
    AllocationFlags flags;
    Allocation *previous = nullptr;
    Allocation *next = nullptr;

    [[nodiscard]] bool isFree() const {
        return flags == AllocationFlags::FREE;
    }
};

template <size_t S>
class DoublyLinkedMemoryAllocator: public MemoryAllocator {
public:
    constexpr static size_t AllocationOverhead = sizeof(Allocation);
    DoublyLinkedMemoryAllocator();
    uintptr_t *allocate(size_t requestedBytes) override;
    void free(void *ptr) override;
    MemoryStats *stats() override;

private:
    constexpr static unsigned floorlog2(unsigned x) {
        return x < 2 ? x : 1+floorlog2(x >> 1);
    }
    constexpr static auto MinimumAllocationSize = sizeof(uintptr_t);
    constexpr static auto MemoryPositions = S / MinimumAllocationSize;
    constexpr static auto MinimumAllocationSizePower = floorlog2(MinimumAllocationSize) - 1;

    Allocation *allocationList = nullptr;
    MemoryStats memoryStats = MemoryStats();
    uintptr_t memory[MemoryPositions] = {};

    Allocation *merge(Allocation *left, Allocation *right);

    [[nodiscard]] Allocation *findBestAllocation(size_t size) const;
};


template<size_t S>
DoublyLinkedMemoryAllocator<S>::DoublyLinkedMemoryAllocator() {
    auto *memoryStartAddress = &memory[0];
    this->allocationList = new (memoryStartAddress) Allocation(S - AllocationOverhead);
}

template<size_t S>
uintptr_t *DoublyLinkedMemoryAllocator<S>::allocate(size_t requestedBytes) {
    const auto requestedBytesWithOverhead = requestedBytes + AllocationOverhead;
    const auto remainderBytes = requestedBytesWithOverhead & (MinimumAllocationSize - 1);
    const auto totalBytesRequired = requestedBytesWithOverhead + remainderBytes;

    auto *allocation = findBestAllocation(totalBytesRequired);

    if (allocation == nullptr) {
        return nullptr;
    }

    const auto bytesLeftAfterAllocation = allocation->size - totalBytesRequired;

    allocation->size = totalBytesRequired;
    allocation->flags = AllocationFlags::USED;

    const auto currentAllocationAddress = ((uintptr_t*) std::addressof(*allocation));
    if (bytesLeftAfterAllocation > 0) {
        const auto nextAllocationAddress = currentAllocationAddress + (totalBytesRequired >> MinimumAllocationSizePower);
        const auto freeBlock = new (nextAllocationAddress) Allocation(bytesLeftAfterAllocation - AllocationOverhead);
        freeBlock->next = allocation->next;
        freeBlock->previous = allocation;
        allocation->next = freeBlock;
    }

    const auto currentAllocationDataAddress = currentAllocationAddress + (AllocationOverhead >> MinimumAllocationSizePower);

    return currentAllocationDataAddress;
}

template<size_t S>
Allocation *DoublyLinkedMemoryAllocator<S>::findBestAllocation(const size_t size) const {
    auto allocation = allocationList;
    Allocation *bestAllocation = nullptr;
    while (allocation != nullptr) {
        if (allocation->isFree() && allocation->size >= size) {
            if (bestAllocation == nullptr || allocation->size < bestAllocation->size) {
                bestAllocation = allocation;
            }
        }
        allocation = allocation->next;
    }

    return bestAllocation;
}

template<size_t S>
void DoublyLinkedMemoryAllocator<S>::free(void *ptr) {
    if (ptr == nullptr) {
        return;
    }
    const auto dataPointerAddress = (uintptr_t*) ptr;
    const auto allocationAddress = dataPointerAddress - (AllocationOverhead >> MinimumAllocationSizePower);
    auto allocation = (Allocation*) allocationAddress;
    if (allocation->flags == AllocationFlags::USED) {
        allocation->flags = AllocationFlags::FREE;

        if (allocation->previous != nullptr && allocation->previous->isFree()) {
            allocation = merge(allocation->previous, allocation);
        }
        if (allocation->next != nullptr && allocation->next->isFree()) {
            allocation = merge(allocation, allocation->next);
        }
    }
}

template<size_t S>
Allocation *DoublyLinkedMemoryAllocator<S>::merge(Allocation *left, Allocation *right) {
    left->size += right->size;
    left->next = right->next;
    if (right->next != nullptr) {
        right->next->previous = left;
    }

    return left;
}

template<size_t S>
MemoryStats *DoublyLinkedMemoryAllocator<S>::stats() {
    memoryStats.size = S;
    memoryStats.used = 0;
    memoryStats.free = 0;
    memoryStats.freeBlocks = 0;
    memoryStats.usedBlocks = 0;
    auto *allocation = allocationList;
    while (allocation != nullptr) {
        if (allocation->isFree()) {
            memoryStats.freeBlocks++;
            memoryStats.free += allocation->size;
        } else {
            memoryStats.usedBlocks++;
            memoryStats.used += allocation->size;
        }
        allocation = allocation->next;
    }
    memoryStats.delta = S - (memoryStats.used + memoryStats.free);

    return &this->memoryStats;
}

#endif //AVR_DOUBLYLINKEDMEMORYALLOCATOR_H
