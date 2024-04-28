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
  uintptr_t max = (uintptr_t)&memory[MemoryPositions - 1];
  uintptr_t min = (uintptr_t)&memory[0];

  Allocation *merge(Allocation *left, Allocation *right);
  [[nodiscard]] Allocation *findBestAllocation(size_t size) const;
  size_t aligned(size_t s);
};

template <size_t S>
size_t DoublyLinkedMemoryAllocator<S>::aligned(size_t s) {
  return (s + 8 - 1) & ~(8 - 1);
}

template <size_t S>
DoublyLinkedMemoryAllocator<S>::DoublyLinkedMemoryAllocator() {
  auto *memoryStartAddress = &memory[0];
  for (size_t i = 0; i < MemoryPositions; i++) {
    memory[i] = 0x12345678;
  }
  this->allocationList = new (memoryStartAddress) Allocation(S - HeaderSize);
}

template <size_t S>
uintptr_t *DoublyLinkedMemoryAllocator<S>::allocate(size_t requestedBytes) {
  auto interruptsWereDisabled = OS::disableInterupts();
  const auto totalBytesRequired = aligned(requestedBytes + HeaderSize);

  auto *allocation = findBestAllocation(totalBytesRequired);

  if (allocation == nullptr) {
    Serial::send('n');
    return nullptr;
  }

  auto bytesLeftAfterAllocation = allocation->size - totalBytesRequired;

  allocation->size = totalBytesRequired;
  allocation->flags = AllocationFlags::USED;

  const auto currentAllocationAddress =
      ((uintptr_t *)std::addressof(*allocation));
  const auto currentAllocationDataAddress =
      currentAllocationAddress + (HeaderSize >> BytesToWordShift);
  const auto sizeInWords =
      (totalBytesRequired - HeaderSize) >> BytesToWordShift;
  for (size_t i = 0; i < sizeInWords; i++) {
    currentAllocationDataAddress[i] = 0x00;
  }

  //    currentAllocationDataAddress[sizeInWords - 1] = 0xDDDDDDDD;

  if (bytesLeftAfterAllocation > 0) {
    const auto nextAllocationAddress =
        currentAllocationAddress + (totalBytesRequired >> BytesToWordShift);
    if ((uintptr_t)nextAllocationAddress > max ||
        (uintptr_t)nextAllocationAddress < min) {
      Serial::send('o');
    }
    const auto freeBlock = new (nextAllocationAddress)
        Allocation(bytesLeftAfterAllocation - HeaderSize);
    freeBlock->next = allocation->next;
    freeBlock->previous = allocation;
    allocation->next = freeBlock;
  }
  if (interruptsWereDisabled) {
    OS::enableInterupts();
  }

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
  auto interruptsWereDisabled = OS::disableInterupts();
  auto *dataPointerAddress = (uintptr_t *)ptr;
  auto *allocationAddress =
      dataPointerAddress - (HeaderSize >> BytesToWordShift);
  auto allocation = (Allocation *)(allocationAddress);
  const auto sizeInWords = allocation->size >> (BytesToWordShift);
  //    const auto safeGuard = allocationAddress[sizeInWords - 1];
  //    if (safeGuard != 0xDDDDDDDD) {
  //      Serial::send('f');
  //      Serial::send(safeGuard);
  //    }
  if (allocation->flags == AllocationFlags::USED) {
    allocation->flags = AllocationFlags::FREE;

    if (allocation->previous != nullptr && allocation->previous->isFree()) {
      allocation = merge(allocation->previous, allocation);
    }
    if (allocation->next != nullptr && allocation->next->isFree()) {
      allocation = merge(allocation, allocation->next);
    }
  }

  for (size_t i = HeaderSize; i < sizeInWords; i++) {
    allocationAddress[i] = 0x21436587;
  }

  if (interruptsWereDisabled) {
    OS::enableInterupts();
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

#endif  // AVR_DOUBLYLINKEDMEMORYALLOCATOR_H
