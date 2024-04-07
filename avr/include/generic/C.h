//
// Created by ghisi on 11.03.24.
//

#ifndef AVR_C_H
#define AVR_C_H

#include "cstddef"
#include "system/OS.h"

extern "C" {
    void *malloc(size_t size) {
        return OS::memalloc(size);
    }

    void free(void *ptr) {
        OS::memfree(ptr);
    }
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
extern "C" void __cxa_pure_virtual(void);

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual(void) {};

#endif //AVR_C_H
