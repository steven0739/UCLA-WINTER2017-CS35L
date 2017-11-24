//
//  main.c
//  tr2b.c
//
//  Created by 杨博超 on 2/16/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Can only pass in two arguments.");
        exit(1);
    }
    
    char * one = argv[1];
    char * two = argv[2];
    
    if (strlen(one) != strlen(two))
    {
        fprintf(stderr, "Arguments should have same length.");
        exit(1);
    }
    
    size_t m;
    size_t n;
    for (m = 0; m < strlen(one); m++)
    {
        for (n = m + 1; n < strlen(one); n++)
        {
            if (one[m] == one[n])
            {
                fprintf(stderr, "Duplicates are not allowed.");
                exit(1);
            }
        }
    }
    
    char cur = getchar();
    while (cur != EOF)
    {
        int check = 0;
        size_t i;
        for (i = 0; i < strlen(one); i++)
        {
            if (cur == one[i])
            {
                putchar(two[i]);
                check = 1;
                break;
            }
        }
        
        if (check == 0)
        {
            putchar(cur);
        }
        
        cur = getchar();
    }
}
