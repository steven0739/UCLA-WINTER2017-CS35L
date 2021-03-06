//
//  main.c
//  randmain
//
//  Created by 杨博超 on 3/10/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include "randcpuid.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <dlfcn.h>

static bool
writebytes (unsigned long long x, int nbytes)
{
    int ndigits = nbytes * 2;
    do
    {
        if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
            return false;
        x >>= 4;
        ndigits--;
    }
    while (0 < ndigits);
    
    return 0 <= putchar ('\n');
}

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
    /* Check arguments.  */
    bool valid = false;
    long long nbytes = 0;
    //long long nbytes;
    if (argc == 2)
    {
        char *endptr;
        errno = 0;
        nbytes = strtoll (argv[1], &endptr, 10);
        if (errno)
            perror (argv[1]);
        else
            valid = !*endptr && 0 <= nbytes;
    }
    if (!valid)
    {
        fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
        return 1;
    }
    
    /* If there's no work to do, don't worry about which library to use.  */
    if (nbytes == 0)
        return 0;
    
    /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
    //void (*initialize) (void);
    unsigned long long (*rand64) (void);
    //void (*finalize) (void);
    void *lib;
    char *error;
    
    if (rdrand_supported ())
    {
        /*
        initialize = hardware_rand64_init;
        rand64 = hardware_rand64;
        finalize = hardware_rand64_fini;
        */
        lib = dlopen("randlibhw.so", RTLD_NOW);
        if (lib == NULL)
        {
            // fprintf(stderr, "%s\n", dlerror());
            printf("Error, unable to open randlibhw.so:%s\n", dlerror());
            exit(1);
        }
        rand64 = dlsym(lib, "hardware_rand64");
        error = dlerror();
        if (error != NULL)
        {
            fprintf (stderr, "Error, unable to retrieve hardware_rand64():%s\n", error);
            exit(1);
        }
    }
    else
    {
        /*
        initialize = software_rand64_init;
        rand64 = software_rand64;
        finalize = software_rand64_fini;
        */
        lib = dlopen("randlibsw.so", RTLD_NOW);
        if (lib == NULL)
        {
            // fprintf(stderr, "%s\n", dlerror());
            printf("Error, unable to open randlibsw.so:%s\n", dlerror());
            exit(1);
        }
        rand64 = dlsym(lib, "software_rand64");
        error = dlerror();
        if (error != NULL)
        {
            // fprintf(stderr, "%s\n", errorstring);
            printf("Error, unable to retrieve software_rand64():%s\n", error);
            exit(1);
        }
    }
    
    int wordsize = sizeof rand64 ();
    int output_errno = 0;
    do
    {
        unsigned long long x = rand64 ();
        int outbytes = nbytes < wordsize ? nbytes : wordsize;
        if (!writebytes (x, outbytes))
        {
            output_errno = errno;
            break;
        }
        nbytes -= outbytes;
    }
    while (0 < nbytes);
    
    if (fclose (stdout) != 0)
        output_errno = errno;
    
    if (output_errno)
    {
        errno = output_errno;
        perror ("output");
        //finalize ();
        return 1;
    }
    
    dlclose(lib);
    return 0;
}












