//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_CPP_H
#define AVR_CPP_H

#include <new>
#include "cstddef"
#include "system/OS.h"

void * operator new(size_t size, std::align_val_t a)
{
    return OS::memalloc(size);
}

void operator delete(void * ptr)
{
    OS::memfree(ptr);
}

void * operator new[](size_t size)
{
    return OS::memalloc(size);
}

void operator delete[](void * ptr)
{
    OS::memfree(ptr);
}

void operator delete(void* ptr, unsigned  int x)
{
    OS::memfree(ptr);
}


#endif //AVR_CPP_H
