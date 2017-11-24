//
//  randlibhw.c
//  randmain
//
//  Created by 杨博超 on 3/10/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include "randlib.h"
#include <immintrin.h>

/* Initialize the hardware rand64 implementation.  */
extern void
hardware_rand64_init (void)
{
}

/* Return a random value, using hardware operations.  */
extern unsigned long long
hardware_rand64 (void)
{
    unsigned long long int x;
    while (! _rdrand64_step (&x))
        continue;
    return x;
}

/* Finalize the hardware rand64 implementation.  */
extern void
hardware_rand64_fini (void)
{
}
