#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    unsigned long size = 0;
    unsigned int nlines = 0;
    struct LINE* Lines = NULL;
};

struct LINE
{
    char* line = NULL;
    unsigned int length = 0;
};

void CreateText(TEXT*, FILE*);

void CreateArrayLines(TEXT*);

int RepAndCount(TEXT* , char, char);

void cwapLine(LINE*, LINE*);

void SortBubble(TEXT*);

void MergeSort(LINE*, int);

void WriteText(TEXT*, FILE*);
