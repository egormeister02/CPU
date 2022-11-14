#include<conio.h>
#include "assm.h"

void CreateCPUtext(const TEXT* assm_text, TEXT* CPU_text)
{    
    ASSERT(assm_text != NULL);
    ASSERT(CPU_text != NULL);
    printf("OK\n");
    printf("%llu\n", (assm_text->nlines + 1) * MAX_CPU_STR);
    CPU_text->buf = (char*)calloc((assm_text->nlines + 1) * MAX_CPU_STR, sizeof(char));
    ASSERT(CPU_text->buf != NULL);
    printf("OK\n");
    CPU_text->nlines = assm_text->nlines + 1;
    printf("OK\n");
    CPU_text->Lines = (LINE*)calloc(CPU_text->nlines, sizeof(LINE));
    ASSERT(CPU_text->Lines != NULL);
    printf("OK\n");
    CPU_text->Lines[0].line = CPU_text->buf;
    printf("OK\n");
}

int SizeVal(int val)
{
    int size_val = 0;
    do
    {
        val /= 10;
        size_val++;
    } while (abs(val) > 0);

    return size_val;
}

void CreateCPUcode(const TEXT* assm_text, TEXT* CPU_text)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_text != NULL);
    printf("OR\n");
    CreateCPUtext(assm_text, CPU_text);
    printf("OR\n");
    int j = 0;
    for (int i = 0; i < (long)assm_text->nlines; i++)
    {   
        //if ((long)assm_text->Lines[i].length < 2) continue;
        puts(assm_text->Lines[i].line);
        char cmd[MAX_SIZE_COMMAND] = "";
        int val = 0;
        int size_str = 0;
        sscanf(assm_text->Lines[i].line, "%s", cmd);
        //printf("OK\n");
        if (strcmp(cmd, "") == 0) continue;
        else if (strcmp(cmd, "push") == 0)
        {
            sscanf((char*)(assm_text->Lines[i].line + 4), "%d", &val);

            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", NAMBER_PUSH, val);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else if (strcmp(cmd, "add") == 0)
        {
            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", NAMBER_ADD, val);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else if (strcmp(cmd, "out") == 0)
        {
            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", NAMBER_OUT, val);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else if (strcmp(cmd, "hlt") == 0)
        {
            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", NAMBER_HLT, val);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else 
        {
            printf("unknown command");
            abort();
        }
    }
}


void TextDumpFunc(const TEXT* text, FILE* LogFile)
{   
    ASSERT(text != NULL);

    fprintf(LogFile, "\n----------------------------------TextDump----------------------------------\n");
    printf("OK\n");
    fprintf(LogFile, "    data pointer = %p\n", text->buf);
    
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
    
    for (size_t index = 0; index < text->nlines; index++)
        {
            fprintf(LogFile, "\t");
            fprintf(LogFile, "[%llu] = ", index);
            
            fprintf(LogFile, "%llu ", text->Lines[index].length);

            fprintf(LogFile, "\n");
        }
        fprintf(LogFile, "    }\n");
}