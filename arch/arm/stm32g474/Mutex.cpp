//
// Created by ghisi on 15.06.24.
//

#include "system/Mutex.h"

extern uint32_t lock_mutex([[maybe_unused]] void *mutex);
extern void unlock_mutex([[maybe_unused]] void *mutex);

uint32_t lock_mutex([[maybe_unused]] void *mutex) {
    uint32_t acquired = 0;
    __asm__ volatile(
            "LDR     r3, =1         \n"
            "LDREX   r5, [%1]       \n"
            "CMP     r5, r3         \n"
            "IT NE                  \n"
            "STREXNE r5, r3, [%1]   \n"
            "DMB                    \n"
            "MOV     %0, r5         \n"
            :"=r"(acquired)
            :"r"(mutex)
            :"r3", "r5");
    return acquired;
}

void unlock_mutex([[maybe_unused]] void *mutex) {
    __asm__ volatile(
            "LDR r3, [%0]   \n"
            "DMB            \n"
            "LDR r3, =0     \n"
            "STR r3, [%0]   \n"
            :
            :"r"(mutex)
            :"r3");
}

bool Mutex::acquire() {
    return lock_mutex(&locked) == 0;
}

void Mutex::release() {
    unlock_mutex(&locked);
}
