//
//  randlibsw.c
//  randmain
//
//  Created by 杨博超 on 3/10/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__ ((__constructor__))
extern void
software_rand64_init (void)
{
    urandstream = fopen ("/dev/urandom", "r");
    if (! urandstream)
        abort ();
}

/* Return a random value, using software operations.  */
extern unsigned long long
software_rand64 (void)
{
    unsigned long long int x;
    if (fread (&x, sizeof x, 1, urandstream) != 1)
        abort ();
    return x;
}

/* Finalize the software rand64 implementation.  */
__attribute__ ((destructor))
extern void
software_rand64_fini (void)
{
    fclose (urandstream);
}

