#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define WINDOWS 1

#define ASSERT(condition)                                         \
if (!(condition)){                                                \
    fprintf(stdout, "Error in %s:\n"                              \
                    "FILE: %s\n"                                  \
                    "LINE: %d\n"                                  \
                    "FUNCTION: %s\n",                             \
           #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
    abort();}

struct TEXT {
    char* buf = NULL;
    size_t size = 0;
    size_t nlines = 0;
    struct LINE* Lines = NULL;
};

struct LINE
{
    char* line = NULL;
    size_t length = 0;
};

const char STR_SEPAR = '\0';

void CreateText(TEXT*, FILE*);

void CreateArrayLines(TEXT*);

int RepAndCount(TEXT* , char, char);

void swapLine(LINE*, LINE*);

void SortBubble(TEXT*);

void MergeSort(LINE*, size_t);

void WriteText(TEXT*, FILE*);
