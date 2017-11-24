//
//  sfrob.c
//
//  Created by Yang Pochao on 2/8/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

// writing the frobcomp function
int frobcmp(char const *a, char const *b)
{
    for ( ; ; a++, b++) // looping through all bytes
    {
        if (*a == ' ' && *b == ' ') // special case when equal
        {
            return 0;
        }
        // compare one by one with the same implementation as memfrob
        else if (*a == ' ' || ( ( (*a) ^ 42) < ( (*b) ^ 42) ) ) // a is less than b case
        {
            return -1;
        }
        else if (*b == ' ' || ( (*b ^ 42) < (*a ^ 42) ) ) // a is larger than b case
        {
            return 1;
        }
    }
}

// report error
void inputErr()
{
    if (ferror(stdin))
    {
        fprintf (stderr, "Error: input wrong");
        exit (1);
    }
}

// pre setup for compar function call in qsort
int compar(const void* args1, const void* args2)
{
    // doing basic settings
    const char* a = *(const char**) args1;
    const char* b = *(const char**) args2;
    // return the compare function we use with arguments all set
    return frobcmp(a, b);
}

int main()
{
    char * singleWord; // word
    char ** multiWords; // words
    char curr;
    char next;
    int letterPosCount; // letter iterator
    int wordPosCount; // words iterator
    
    singleWord = (char*) malloc(sizeof (char));
    multiWords = (char**) malloc(sizeof (char*));
    curr = getchar();
    inputErr();
    next = getchar();
    inputErr();
    letterPosCount = 0;
    wordPosCount = 0;
    
    while(curr != EOF && !ferror(stdin))
    {
        // basic settings
        singleWord[letterPosCount] = curr;
        char * newLetterPos = realloc(singleWord, (letterPosCount + 2) * sizeof(char));
        
        if (newLetterPos != NULL)
        {
            singleWord = newLetterPos;
        }
        else
        {
            free(singleWord);
            fprintf( stderr, "Error: memory allocation failed");
            exit (1);
        }
        
        if (curr == ' ')
        {
            multiWords[wordPosCount] = singleWord;
            char ** newWordPos = realloc(multiWords, (wordPosCount + 2) * sizeof(char *));
            
            if (newWordPos != NULL)
            {
                multiWords = newWordPos;
                wordPosCount ++;
                
                singleWord = NULL;
                singleWord = (char*) malloc(sizeof (char));
                letterPosCount = -1;
            }
            else
            {
                free(multiWords);
                fprintf( stderr, "Error: memory allocation failed");
                exit (1);
            }
            
            if (next == ' ')
            {
                while (curr == ' ')
                {
                    curr = getchar();
                    inputErr();
                }
                next = getchar();
                inputErr();
                letterPosCount = 0; // back to 0
                continue;
            }
            
            if (next == EOF)
            {
                break;
            }
        }
        
        if (next == EOF)
        {
            if (curr != ' ') // special case checking
            {
                letterPosCount++;
                curr = ' ';
                singleWord[letterPosCount] = curr;
                continue;
            }
        }

        curr = next;
        next = getchar();
        inputErr();
        letterPosCount++;
    }
    
    qsort(multiWords, wordPosCount, sizeof(char *), compar);
    
    for (size_t m = 0; m < wordPosCount; m++)
    {
        for (int n = 0; ; n++)
        {
            if (multiWords[m][n] == EOF)
            {
                fprintf( stderr, "Error: output wrong");
                exit (1);
            }
            else
            {
                putchar(multiWords[m][n]);
                if (multiWords[m][n] == ' ')
                {
                    break;
                }
            }
        }
    }
    
    for (int i = 0; i < wordPosCount; i++)
    {
        free(multiWords[i]);
    }
    free(multiWords);
    
    exit (0);
}




