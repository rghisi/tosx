//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_MEMORYALLOCATOR_H
#define AVR_MEMORYALLOCATOR_H

#include <cstdint>
#include <cstddef>

class MemoryStats {
public:
    size_t size;
    size_t used;
    size_t free;
    size_t usedBlocks;
    size_t freeBlocks;
};

class MemoryAllocator {
public:
    virtual uintptr_t *allocate(size_t requestedBytes) = 0;
    virtual void free(void *ptr) = 0;
    virtual MemoryStats *stats() = 0;
};


#endif //AVR_MEMORYALLOCATOR_H
