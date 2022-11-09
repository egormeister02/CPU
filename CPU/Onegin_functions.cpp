#include "Onegin.h"

void CreateText(TEXT* text, FILE* file) 
{
    ASSERT(text != NULL);
    ASSERT(file != NULL);

    fseek(file, 0, SEEK_END);
    text->size = ftell(file);
    rewind(file);

    text->buf = (char*)calloc(text->size, sizeof(char));

    ASSERT(text->buf != NULL);

    fread(text->buf,sizeof(char), text->size, file);

    text->nlines = RepAndCount(text, '\n', '\n');

    CreateArrayLines(text);
}

void CreateArrayLines(TEXT* text) 
{
    text->Lines = (LINE*)calloc(text->nlines, sizeof(LINE));

    ASSERT(text->Lines != NULL);
    
    text->Lines[0].line = (char*)text->buf;
    
    for (int i = 0, k = 1; i < text->size; i++) 
    {
        if (*(text->buf + i) == '\n') 
        {
            text->Lines[k].line = (char*)(text->buf + i + 1);
            text->Lines[k-1].length = (size_t)(text->Lines[k].line - text->Lines[k-1].line);
            k++;
        }
    }
}

int RepAndCount(TEXT* text, char a, char b) 
{
    ASSERT(text != NULL);

    int count = 0;
    for (int i = 0; i < text->size; i++) {
        if (*(text->buf + i) == a) {
            *(text->buf + i) = b;
            count++;
        }
    }
    return count;
}

void swapLine(LINE* a,LINE* b) 
{
    ASSERT(a != NULL);
    ASSERT(b != NULL);

    LINE c = *a;
    *a = *b;
    *b = c;
}

void SortBubble(TEXT* text) 
{
    ASSERT(text != NULL);

    for (int i = 1; i < text->nlines; i++)
        for (int j = 0; j < text->nlines - i; j++)
            if (strcmp(text->Lines[j].line, text->Lines[j+1].line) > 0)
                swapLine(&text->Lines[j], &text->Lines[j+1]);
}

void WriteText(TEXT* text, FILE* file)
{   
    ASSERT(text != NULL);
    ASSERT(file != NULL);

    for (int i = 0; i < text->nlines; i++) {
        fwrite(text->Lines[i].line, text->Lines[i].length, sizeof(char), file);
    }
}

void MergeSort(LINE* Lines, int fsize)
{
    ASSERT(Lines != NULL);

    if (fsize < 2)return;

    MergeSort(Lines, fsize / 2);
    MergeSort(&Lines[fsize / 2], fsize - (fsize / 2));

    LINE* buf = (LINE*)calloc(fsize, sizeof(LINE));

    int idbuf = 0;
    int idl = 0;
    int idr = fsize / 2 ;

    while ((idl < fsize / 2) && (idr < fsize))
        if (strcmp(Lines[idl].line, Lines[idr].line) < 0) 
            buf[idbuf++] = Lines[idl++];
        else
            buf[idbuf++] = Lines[idr++];
    
    while (idl < fsize / 2) 
        buf[idbuf++] = Lines[idl++];

    while (idr < fsize) 
        buf[idbuf++] = Lines[idr++];

    for (idl = 0; idl < fsize; idl++) 
        Lines[idl] = buf[idl];
        
    free(buf);
}