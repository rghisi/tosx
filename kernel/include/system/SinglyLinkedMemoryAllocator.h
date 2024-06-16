//
// Created by ghisi on 22.03.23.
//

#ifndef AVR_SINGLYLINKEDMEMORYALLOCATOR_H
#define AVR_SINGLYLINKEDMEMORYALLOCATOR_H

#include "MemoryAllocator.h"
#include "cstddef"
#include "cstdint"

enum class AllocationFlags {
    FREE = 0x00, USED = 0xFF
};

class Allocation {
public:
    explicit Allocation(size_t size) {
        this->size = size + sizeof(Allocation);
        flags = AllocationFlags::FREE;
    }

    size_t size;
    AllocationFlags flags;
    uint_fast16_t ownerId;
    Allocation *next = nullptr;

    [[nodiscard]] bool isFree() const { return flags == AllocationFlags::FREE; }
};

class SinglyLinkedMemoryAllocator : public MemoryAllocator {
public:
    SinglyLinkedMemoryAllocator(uint32_t startAddress, uint32_t endAddress);

    uintptr_t *allocate(size_t requestedBytes, uint_fast16_t ownerId) override;

    void free(void *ptr) override;

    MemoryStats *stats() override;

    size_t UsedMemory(uint_fast16_t ownerId) override;
    void Dump() override;

private:
    constexpr static auto AddressSize = sizeof(uintptr_t);
    constexpr static auto HeaderSize = sizeof(Allocation);
    constexpr static auto BytesToWordShift = AddressSize >> 1;
    size_t totalSize;

    Allocation *allocationList = nullptr;
    MemoryStats memoryStats = MemoryStats();


    Allocation *merge(Allocation *left, Allocation *right);

    [[nodiscard]] Allocation *findBestAllocation(size_t size);

    size_t aligned(size_t s);

    void MergeFreeBlocks();
};

size_t SinglyLinkedMemoryAllocator::aligned(size_t s) {
    return (s + AddressSize - 1l) & ~(AddressSize - 1l);
}

SinglyLinkedMemoryAllocator::SinglyLinkedMemoryAllocator(uint32_t startAddress, uint32_t endAddress) {
    totalSize = endAddress - startAddress;
    auto *memoryStartAddress = (uint32_t *) startAddress;
    this->allocationList = new(memoryStartAddress) Allocation(totalSize - HeaderSize);
    memoryStats.size = totalSize;
    memoryStats.used = HeaderSize;
    memoryStats.free = totalSize - HeaderSize;
    memoryStats.usedBlocks = 0;
    memoryStats.freeBlocks = 1;
}

uintptr_t *SinglyLinkedMemoryAllocator::allocate(size_t requestedBytes, uint_fast16_t ownerId) {
    const auto totalBytesRequired = aligned(requestedBytes + HeaderSize);

    auto *allocation = findBestAllocation(totalBytesRequired);

    if (allocation == nullptr) {
        Dump();
        return nullptr;
    }

    allocation->ownerId = ownerId;

    auto bytesLeftAfterAllocation = allocation->size - totalBytesRequired;

    allocation->size = totalBytesRequired;
    allocation->flags = AllocationFlags::USED;

    const auto currentAllocationAddress = (uintptr_t) allocation;
    const auto currentAllocationDataAddress = (uintptr_t*) (currentAllocationAddress + HeaderSize);

    if (bytesLeftAfterAllocation > (HeaderSize << 1)) {
        const auto nextAllocationAddress = (uintptr_t*)(currentAllocationAddress + totalBytesRequired);
        const auto freeBlock = new(nextAllocationAddress) Allocation(bytesLeftAfterAllocation - HeaderSize);
        freeBlock->next = allocation->next;
        allocation->next = freeBlock;
    } else {
        allocation->size += bytesLeftAfterAllocation;
        memoryStats.freeBlocks--;
    }

    memoryStats.used += allocation->size;
    memoryStats.free -= allocation->size;
    memoryStats.usedBlocks++;

    return currentAllocationDataAddress;
}

Allocation *SinglyLinkedMemoryAllocator::findBestAllocation(const size_t size) {
    auto *allocation = allocationList;
    Allocation *bestAllocation = nullptr;
    while (allocation != nullptr) {
        if (allocation->isFree()) {
            while (allocation->next != nullptr && allocation->next->isFree()) {
                merge(allocation, allocation->next);
                memoryStats.freeBlocks--;
            }
            if (allocation->size >= size) {
                if (bestAllocation == nullptr || allocation->size < bestAllocation->size) {
                    bestAllocation = allocation;
                }
            }
        }
        allocation = allocation->next;
    }

    return bestAllocation;
}

void SinglyLinkedMemoryAllocator::free(void *ptr) {
    if (ptr == nullptr) {
        printf("\r\n\t\tWARNING: Freeing null memory allocation\r\n");
        return;
    }
    auto *dataPointerAddress = (uintptr_t *) ptr;
    auto *allocationAddress = dataPointerAddress - (HeaderSize >> BytesToWordShift);
    auto allocation = (Allocation *) (allocationAddress);

    if (allocation->flags == AllocationFlags::USED) {
        allocation->flags = AllocationFlags::FREE;
        allocation->ownerId = UINT16_MAX;
        memoryStats.used -= allocation->size;
        memoryStats.free += allocation->size;
        memoryStats.usedBlocks--;
        memoryStats.freeBlocks++;
    }

    if (memoryStats.freeBlocks >= 5) {
        MergeFreeBlocks();
    }
}

void SinglyLinkedMemoryAllocator::MergeFreeBlocks() {
    auto allocation = allocationList;
    while (allocation != nullptr) {
        while (allocation->next != nullptr && allocation->isFree() && allocation->next->isFree()) {
            merge(allocation, allocation->next);
            memoryStats.freeBlocks--;
        }
        allocation = allocation->next;
    }
}

Allocation *SinglyLinkedMemoryAllocator::merge(Allocation *left, Allocation *right) {
    left->size += right->size;
    left->next = right->next;

    return left;
}

MemoryStats *SinglyLinkedMemoryAllocator::stats() {
    return &this->memoryStats;
}

size_t SinglyLinkedMemoryAllocator::UsedMemory(uint_fast16_t ownerId) {
    size_t size = 0;
    auto allocation = allocationList;
    while (allocation != nullptr) {
        if (!allocation->isFree() && allocation->ownerId == ownerId) {
            size += allocation->size;
        }
        allocation = allocation->next;
    }

    return size;
}

void SinglyLinkedMemoryAllocator::Dump() {
    printf("Bytes\r\n");
    printf(" Size:\t%u\r\n", memoryStats.size);
    printf(" Total:\t%u\r\n", memoryStats.used + memoryStats.free);
    printf(" Used:\t%u\r\n", memoryStats.used);
    printf(" Free:\t%u\r\n", memoryStats.free);
    printf("Blocks\r\n");
    printf(" Total:\t%u\r\n", memoryStats.usedBlocks + memoryStats.freeBlocks);
    printf(" Used:\t%u\r\n", memoryStats.usedBlocks);
    printf(" Free:\t%u\r\n", memoryStats.freeBlocks);

    auto allocation = allocationList;
    auto block = 0u;
    while (allocation != nullptr) {
        const auto allocationAddress = (uintptr_t) allocation;
        const auto flags = allocation->flags == AllocationFlags::FREE ? 'F' : 'U';
        Serial::send('\t');
        Serial::send(block);
        Serial::send(' ');
        Serial::send(allocationAddress);
        Serial::send(' ');
        Serial::send(allocation->size);
        Serial::send('(');
        Serial::send(flags);
        Serial::send(')');
//        printf("\t%u %u %u(%c)", block, allocationAddress, allocation->size, flags);
        if (allocation->next != nullptr) {
            const auto nextAllocationAddress = (uintptr_t) allocation->next;
            Serial::send(' ');
            Serial::send(nextAllocationAddress);
//            printf(" %u", nextAllocationAddress);
        }
        Serial::send("\r\n");
        allocation = allocation->next;
        block++;
    }
}


#endif  // AVR_SINGLYLINKEDMEMORYALLOCATOR_H
