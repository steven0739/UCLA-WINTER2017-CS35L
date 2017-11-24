//
//  randcpuid.c
//  randmain
//
//  Created by 杨博超 on 3/10/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include "randcpuid.h"
#include <cpuid.h>

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
    struct cpuid result;
    asm ("cpuid"
         : "=a" (result.eax), "=b" (result.ebx),
         "=c" (result.ecx), "=d" (result.edx)
         : "a" (leaf), "c" (subleaf));
    return result;
}

/* Return true if the CPU supports the RDRAND instruction.  */
extern _Bool
rdrand_supported (void)
{
    struct cpuid extended = cpuid (1, 0);
    return (extended.ecx & bit_RDRND) != 0;
}
