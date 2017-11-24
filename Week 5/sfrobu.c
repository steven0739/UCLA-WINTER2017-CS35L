//
//  sfrobu.c
//
//  Created by Yang Pochao on 2/8/17.
//  Copyright © 2017 Yang Pochao 204631541. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
//add those files according to codewiki

//write the frobcomp function
int frobcmp1(char const *a, char const *b)
{
    for ( ; ; a++, b++) //loop through all bytes
    {
        char da = *a ^ 42;
        char db = *b ^ 42;
        char uda = (unsigned char) da;
        char udb = (unsigned char) db;
        uda = toupper(uda);
        udb = toupper(udb);
        
        if (*a == ' ' && *b == ' ') //special case when equal
        {
            return 0;
        }
        //compare one by one with the same implementation as specified in memfrob
        else if (*a == ' ' || uda < udb ) //a is less than b case
        {
            return -1;
        }
        else if (*b == ' ' || uda > udb ) //a is larger than b case
        {
            return 1;
        }
    }
}

int frobcmp0(char const *a, char const *b)
{
    for ( ; ; a++, b++) //loop through all bytes
    {
        if (*a == ' ' && *b == ' ') //special case when equal
        {
            return 0;
        }
        //compare one by one with the same implementation as specified in memfrob
        else if (*a == ' ' || ( ( (*a) ^ 42) < ( (*b) ^ 42) ) ) //a is less than b case
        {
            return -1;
        }
        else if (*b == ' ' || ( (*b ^ 42) < (*a ^ 42) ) ) //a is larger than b case
        {
            return 1;
        }
    }
}

//report reading error
void inputErr(ssize_t readnums)
{
    if (readnums == -1) //read will return -1 if anything wrong
    {
        fprintf (stderr, "Error: read wrong");
        exit (1);
    }
}

//pre setup for compar function call in qsort
int compar1(const void* args1, const void* args2)
{
    //set up pointer to a pointer
    const char* a = *(const char**) args1;
    const char* b = *(const char**) args2;
    //return the compare function we use with arguments all set
    return frobcmp1(a, b);
}

int compar0(const void* args1, const void* args2)
{
    //set up pointer to a pointer
    const char* a = *(const char**) args1;
    const char* b = *(const char**) args2;
    //return the compare function we use with arguments all set
    return frobcmp0(a, b);
}

int main(int argc, char *argv[])
{
    int file = 0;
    struct stat fileStat;
    if (fstat(file, &fileStat) < 0)
    {
        fprintf (stderr, "Error: system call of file wrong.");
        exit(1);
    }
    
    char * charInFile;
    char ** multiWords; // array of array of char
    size_t wordPosCount = 0;
    
    if (S_ISREG(fileStat.st_mode)) //check whether the input is a regular file
    {
        charInFile = (char*) malloc(fileStat.st_size * sizeof(char)); //allocate size as size of file
        ssize_t readnum = read(0, charInFile, fileStat.st_size);
        inputErr(readnum);
        
        int wordsnum = 1;
        
        if (readnum > 0)
        {
            charInFile[fileStat.st_size - 1] = ' '; //set the end of the file to space, which was a null byte
            size_t m;
            for (m = 0; m < fileStat.st_size; m++)
            {
                if (charInFile[m] == ' ') //count how many words in multiwords
                {
                    wordsnum += 1;
                }
            }
        }
        
        multiWords = (char**) malloc(wordsnum * sizeof (char*)); //initialize
        
        size_t n;
        int sep = 0;
        for (n = 0; n < fileStat.st_size; n++) //point to the start of each single word, using space as seperation
        {
            if (sep == 0 && charInFile[n] != ' ')
            {
                multiWords[wordPosCount] = &charInFile[n];
                wordPosCount++;
                sep = 1;
            }
            else if (sep == 1 && charInFile[n] == ' ')
            {
                sep = 0;
            }
        }
    }
    
    else //not a regular file
    {
        multiWords = (char**) malloc(sizeof (char*)); //initialize
    }
    
    //
    //start doing dynamical allocate now
    char * singleWord; // array of char
    char curr[1]; //subtle change because of read function
    char next[1];
    int letterPosCount = 0; // char in array iterator
    
    singleWord = (char*) malloc(sizeof (char)); //initialize
    
    ssize_t readCurr = read(0, curr, 1);
    inputErr(readCurr);
    ssize_t readNext = read(0, next, 1);
    inputErr(readNext);

    while(readCurr > 0)
    {
        //basic settings
        singleWord[letterPosCount] = curr[0]; //put in the char
        char * newLetterPos = realloc(singleWord, (letterPosCount + 2) * sizeof(char)); //allocate new space
        
        if (newLetterPos != NULL) //re-allocate
        {
            singleWord = newLetterPos;
        }
        else //error check
        {
            free(singleWord);
            fprintf( stderr, "Error: memory allocation failed");
            exit (1);
        }
        
        if (curr[0] == ' ') //end of a word, complete char array
        {
            multiWords[wordPosCount] = singleWord; //put the word into multiwords
            char ** newWordPos = realloc(multiWords, (wordPosCount + 2) * sizeof(char *)); //allocate new space
            
            if (newWordPos != NULL) //re-allocate
            {
                multiWords = newWordPos;
                wordPosCount++;
                
                //reset the char array
                singleWord = NULL;
                singleWord = (char*) malloc(sizeof (char));
                letterPosCount = -1; //since there will be an increment later
            }
            else //error check
            {
                free(multiWords);
                fprintf( stderr, "Error: memory allocation failed");
                exit (1);
            }
            
            if (next[0] == ' ') //special case when continuous sp
            {
                while (curr[0] == ' ') //increment curr to effective one, not space
                {
                    readCurr = read(0, curr, 1);
                    inputErr(readCurr);
                }
                //re-set up
                readNext = read(0, next, 1);
                inputErr(readNext);
                letterPosCount++; // back to 0
                continue;
            }
            
            if (readNext == 0) //curr is sp, next is EOF
            {
                break; //end here
            }
        }
        
        if (readNext == 0) //curr is not sp, but next is EOF
        {
            //need to add a sp at the end of the array
            curr[0] = ' '; //so first change curr to sp
            letterPosCount++;
            continue; //to store this array(word) in multiwords
        }

        //re-set up
        curr[0] = next[0];
        readNext = read(0, next, 1);
        inputErr(readNext);
        letterPosCount++;
    }
    
    if (argc == 2 && strcmp(argv[1], "-f") == 0) //call sort accoerding to -f option
    {
        qsort(multiWords, wordPosCount, sizeof(char *), compar1);
    }
    else
    {
        qsort(multiWords, wordPosCount, sizeof(char *), compar0);
    }
    
    size_t m;
    for (m = 0; m < wordPosCount; m++) //output
    {
        long wordsize = 0;
        size_t n;
        for (n = 0; ; n++)
        {
            wordsize += 1;
            if (multiWords[m][n] == ' ')
            {
                break;
            }
        }
        if (write(1, multiWords[m], wordsize) == 0)
        {
            fprintf (stderr, "Error: write wrong.");
            exit(1);
        }
    }
    
    free(multiWords);
    
    exit (0);
}
