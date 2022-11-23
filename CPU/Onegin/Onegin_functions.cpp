#include<conio.h>
#include "Onegin.h"

void CreateText(TEXT* text, FILE* file) 
{
    ASSERT(text != NULL);
    ASSERT(file != NULL);

    fseek(file, 0, SEEK_END);
    text->size = ftell(file);
    rewind(file);

    text->buf = (char*)calloc(text->size + 1, sizeof(char));
    ASSERT(text->buf != NULL);

    text->size = fread(text->buf, sizeof(char), text->size, file);

    if (text->buf[text->size-1] != '\n')
    {
        text->buf[text->size] = '\n';
        text->size += 1;
    }

    text->nlines = RepAndCount(text, '\n', STR_SEPAR);
    CreateArrayLines(text);
}

void DtorText(TEXT* text)
{
    ASSERT(text != NULL);

    free(text->buf);
    text->buf = NULL;
    text->nlines = 0;
    text->size = 0;
    free(text->Lines);
    text->Lines = NULL;  
}

void CreateArrayLines(TEXT* text) 
{
    ASSERT(text != NULL);

    text->Lines = (LINE*)calloc(text->nlines, sizeof(LINE));
    ASSERT(text->Lines != NULL);
    
    text->Lines[0].line = text->buf;
    
    for (int i = 0, k = 1; i < (long)text->size; i++) 
    {
        if (*(text->buf + i) == STR_SEPAR) 
        {
            text->Lines[k].line = (char*)(text->buf + i + 1);
            text->Lines[k-1].length = (text->Lines[k].line - text->Lines[k-1].line);

#if SKIP_EMPTY_STR
            if (IsEmpty(&text->Lines[k-1]))
                text->Lines[k-1].line = (char*)(text->buf + i + 1);
    
            else 
                k++;
#else
            k++;
#endif
        }
        text->nlines = k-1;
    }
}

int RepAndCount(TEXT* text, char a, char b) 
{
    ASSERT(text != NULL);

    int count = 0;
    for (int i = 0; i < (long)text->size; i++) {
        if (*(text->buf + i) == a)
        {
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

    for (int i = 1; i < (long)text->nlines; i++)
        for (int j = 0; j < (long)text->nlines - i; j++)
            if (strcmp(text->Lines[j].line, text->Lines[j+1].line) > 0)
                swapLine(&text->Lines[j], &text->Lines[j+1]);
}

void WriteText(TEXT* text, FILE* file)
{   
    ASSERT(text != NULL);
    ASSERT(file != NULL);

    for (int i = 0; i < (long)text->nlines; i++) {
        fwrite(text->Lines[i].line, text->Lines[i].length, sizeof(char), file);
    }
}

void MergeSort(LINE* Lines, size_t fsize,const mode mode)
{
    ASSERT(Lines != NULL);

    if (fsize < 2)return;

    MergeSort(Lines, fsize / 2, mode);
    MergeSort(&Lines[fsize / 2], fsize - (fsize / 2), mode);

    LINE* buf = (LINE*)calloc(fsize, sizeof(LINE));

    size_t idbuf = 0;
    size_t idl = 0;
    size_t idr = fsize / 2 ;

    while ((idl < fsize / 2) && (idr < fsize))
        if (Mystrcmp(&Lines[idl], &Lines[idr], mode) < 0)
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

int Mystrcmp(const LINE* line1, const LINE* line2, const mode mode)
{
    ASSERT(line1 != NULL);
    ASSERT(line2 != NULL);

    if (mode == LEFT)
    {
        int i = 0;
        int j = 0;
        while ((i < (long)line1->length) && (j < (long)line2->length))
        {
            if (IsLetter(line1->line[i]))
            {
                if (IsLetter(line2->line[j]))
                {
                    if (lowercase(line1->line[i]) > lowercase(line2->line[j]))
                        return 1;

                    if (lowercase(line1->line[i]) < lowercase(line2->line[j]))
                        return -1;

                    i++; j++; continue;
                }   
                else 
                {
                    j++; continue;
                }
            }
            else
            {
                i++; continue;
            }
        }
        if (line1->length > line2->length)
            return 1;
            
        if (line1->length < line2->length)
            return -1;
        return 0;
    }

    if (mode == RIGHT)
    {
        int i = ((long)line1->length) - 1;
        int j = ((long)line2->length) - 1;
        while ((i >= 0) && (j >= 0))
        {
            if (IsLetter(line1->line[i]))
            {
                if (IsLetter(line2->line[j]))
                {
                    if (lowercase(line1->line[i]) > lowercase(line2->line[j]))
                        return 1;
                        
                    if (lowercase(line1->line[i]) < lowercase(line2->line[j]))
                        return -1;

                    i--; j--; continue;
                }   
                else 
                {
                    j--; continue;
                }
            }
            else
            {
                i--; continue;
            }
        }
        if (line1->length > line2->length)
            return 1;

        if (line1->length < line2->length)
            return -1;
        return 0;
    }
}

int IsEmpty(const LINE* line)
{
    for (int i = 0; i < ((long)line->length) - 1; i++)
        if (line->line[i] != ' ') return 0;
    
    return 1;
}

int IsLetter(const char a)
{
    return (((a >= 'A') && (a <= 'Z')) || ((a >= 'a') && (a <= 'z')));
}

char lowercase(char a)
{
    if ((a >= 'A') && (a <= 'Z'))
        return a + 'a' - 'A';
    return a; 
}

void TextDumpFunc(const TEXT* text, FILE* LogFile)
{   
    ASSERT(text != NULL);
    ASSERT(LogFile != NULL);

    fprintf(LogFile, "\n----------------------------------TextDump----------------------------------\n");
    fprintf(LogFile, "    buf pointer = %p\n", text->buf);
    
        fprintf(LogFile, "    size         = %llu\n", text->size);
        fprintf(LogFile, "    nlines     = %llu\n", text->nlines);

        fprintf(LogFile, "    {\n");
        for (size_t index = 0; index < text->size; index++)
        {
            fprintf(LogFile, "\t");
            fprintf(LogFile, "[%llu] = ", index);
            
            fprintf(LogFile, "%c ", text->buf[index]);

            fprintf(LogFile, "\n");
        }
        fprintf(LogFile, "    }\n");

    fprintf(LogFile, "}\n");
    fprintf(LogFile, "\n---------------------------------------------------------------------------\n");
    fprintf(LogFile, "\n------------------------------LinesLengthDump------------------------------\n");
    
    for (size_t index = 0; index < text->nlines; index++)
        {
            fprintf(LogFile, "\t");
            fprintf(LogFile, "[%llu] = ", index);
            
            fprintf(LogFile, "%llu ", text->Lines[index].length);

            fprintf(LogFile, "\n");
        }
        fprintf(LogFile, "    }\n");
}
