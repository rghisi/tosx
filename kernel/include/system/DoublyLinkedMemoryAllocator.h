//
// Created by ghisi on 22.03.23.
//

#ifndef AVR_DOUBLYLINKEDMEMORYALLOCATOR_H
#define AVR_DOUBLYLINKEDMEMORYALLOCATOR_H

#include "MemoryAllocator.h"
#include "cstddef"
#include "cstdint"

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

  [[nodiscard]] bool isFree() const { return flags == AllocationFlags::FREE; }
};

template <size_t S>
class DoublyLinkedMemoryAllocator : public MemoryAllocator {
 public:
  DoublyLinkedMemoryAllocator();
  uintptr_t *allocate(size_t requestedBytes) override;
  void free(void *ptr) override;
  MemoryStats *stats() override;

 private:
  constexpr static auto AddressSize = sizeof(uintptr_t);
  constexpr static auto HeaderSize = sizeof(Allocation);
  constexpr static auto MemoryPositions = S / AddressSize;
  constexpr static auto BytesToWordShift = AddressSize >> 1;

  Allocation *allocationList = nullptr;
  MemoryStats memoryStats = MemoryStats();
  uintptr_t memory[MemoryPositions] = {};


  Allocation *merge(Allocation *left, Allocation *right);
  [[nodiscard]] Allocation *findBestAllocation(size_t size) const;
  size_t aligned(size_t s);
};

template <size_t S>
size_t DoublyLinkedMemoryAllocator<S>::aligned(size_t s) {
  return (s + AddressSize - 1l) & ~(AddressSize - 1l);
}

template <size_t S>
DoublyLinkedMemoryAllocator<S>::DoublyLinkedMemoryAllocator() {
  auto *memoryStartAddress = &memory[0];
  for (size_t i = 0; i < MemoryPositions; i++) {
    memory[i] = 0x00;
  }
  this->allocationList = new (memoryStartAddress) Allocation(S - HeaderSize);
  memoryStats.size = S;
  memoryStats.used = HeaderSize;
  memoryStats.free = S - HeaderSize;
  memoryStats.usedBlocks = 0;
  memoryStats.freeBlocks = 1;
}

template <size_t S>
uintptr_t *DoublyLinkedMemoryAllocator<S>::allocate(size_t requestedBytes) {
  const auto totalBytesRequired = aligned(requestedBytes + HeaderSize);

  auto *allocation = findBestAllocation(totalBytesRequired);

  if (allocation == nullptr) {
    return nullptr;
  }

  auto bytesLeftAfterAllocation = allocation->size - totalBytesRequired;

  allocation->size = totalBytesRequired;
  allocation->flags = AllocationFlags::USED;

  const auto currentAllocationAddress = ((uintptr_t *)std::addressof(*allocation));
  const auto currentAllocationDataAddress = &currentAllocationAddress[(HeaderSize >> BytesToWordShift)];
  const auto sizeInWords = (totalBytesRequired - HeaderSize) >> BytesToWordShift;
  for (size_t i = 0; i < sizeInWords; i++) {
    currentAllocationDataAddress[i] = 0x00;
  }

  if (bytesLeftAfterAllocation > (HeaderSize << 1)) {
    const auto nextAllocationAddress = &currentAllocationAddress[(totalBytesRequired >> BytesToWordShift)];
    const auto freeBlock = new (nextAllocationAddress) Allocation(bytesLeftAfterAllocation - HeaderSize);
    freeBlock->next = allocation->next;
    freeBlock->previous = allocation;
    allocation->next = freeBlock;
  } else {
    memoryStats.freeBlocks--;
  }

  memoryStats.used += allocation->size;
  memoryStats.free -= allocation->size;
  memoryStats.usedBlocks++;

  return currentAllocationDataAddress;
}

template <size_t S>
Allocation *DoublyLinkedMemoryAllocator<S>::findBestAllocation(
    const size_t size) const {
  auto allocation = allocationList;
  Allocation *bestAllocation = nullptr;
  while (allocation != nullptr) {
    if (allocation->isFree() && allocation->size >= size) {
      if (bestAllocation == nullptr ||
          allocation->size < bestAllocation->size) {
        bestAllocation = allocation;
      }
    }
    allocation = allocation->next;
  }

  return bestAllocation;
}

template <size_t S>
void DoublyLinkedMemoryAllocator<S>::free(void *ptr) {
  if (ptr == nullptr) {
    return;
  }
  auto *dataPointerAddress = (uintptr_t *)ptr;
  auto *allocationAddress = dataPointerAddress - (HeaderSize >> BytesToWordShift);
  auto allocation = (Allocation *)(allocationAddress);

  if (allocation->flags == AllocationFlags::USED) {
    allocation->flags = AllocationFlags::FREE;
    memoryStats.used -= allocation->size;
    memoryStats.free += allocation->size;
    memoryStats.usedBlocks--;
    memoryStats.freeBlocks++;
//    while (allocation->previous != nullptr && allocation->previous->isFree()) {
//      allocation = merge(allocation->previous, allocation);
//      memoryStats.freeBlocks--;
//    }

    while (allocation->next != nullptr && allocation->next->isFree()) {
      merge(allocation, allocation->next);
      memoryStats.freeBlocks--;
    }
  }
}

template <size_t S>
Allocation *DoublyLinkedMemoryAllocator<S>::merge(Allocation *left,
                                                  Allocation *right) {
  left->size += right->size;
  left->next = right->next;
  if (right->next != nullptr) {
    right->next->previous = left;
  }

  return left;
}

template <size_t S>
MemoryStats *DoublyLinkedMemoryAllocator<S>::stats() {
  return &this->memoryStats;
}

#endif  // AVR_DOUBLYLINKEDMEMORYALLOCATOR_H
