#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SKIP_EMPTY_STR 1
const char STR_SEPAR = '\n';

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

enum mode
{
    LEFT = 0,
    RIGHT = 1
};



void CreateText(TEXT*, FILE*);

void DtorText(TEXT*);

void CreateArrayLines(TEXT*);

int RepAndCount(TEXT* , char, char);

void swapLine(LINE*, LINE*);

void SortBubble(TEXT*);

void MergeSort(LINE*, size_t, const mode);

void TextDumpFunc(const TEXT*, FILE*);

void WriteText(TEXT*, FILE*);

int Mystrcmp(const LINE*, const LINE*, const mode);

int IsLetter(const char);

char lowercase(const char);

int IsEmpty(const LINE*);
