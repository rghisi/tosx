//
// Created by ghisi on 21.03.24.
//

#ifndef ARM_CONTEXTSWITCHING_H
#define ARM_CONTEXTSWITCHING_H

#define saveContext() \
asm volatile ( \
        "PUSH          {r4-r11} \n\t" \
        )

#define restoreContext() \
    asm volatile (  \
        "POP           {r4-r11} \n\t"  \
        )

#endif //ARM_CONTEXTSWITCHING_H
