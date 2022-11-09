#include "assm.h"

void CreateCPUtext(const TEXT* assm_text, TEXT* CPU_text)
{    
    ASSERT(assm_text != NULL);
    ASSERT(CPU_text != NULL);
    
    CPU_text->buf = (char*)calloc(/*(assm_text->nlines + 1)*/5 * MAX_CPU_STR, sizeof(char));
    ASSERT(CPU_text->buf != NULL);

    CPU_text->nlines = assm_text->nlines + 1;

    CPU_text->Lines = (LINE*)calloc(CPU_text->nlines, sizeof(LINE));
    ASSERT(CPU_text->Lines != NULL);

    CPU_text->Lines[0].line = CPU_text->buf;
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

void CreateSoftCPU(const TEXT* assm_text, TEXT* CPU_text)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_text != NULL);

    CreateCPUtext(assm_text, CPU_text);
    
    for (int i = 0; i < assm_text->nlines; i++)
    {
        char cmd[MAX_SIZE_COMMAND] = "";
        int val = 0;
        int size_str = 0;
        sscanf(assm_text->Lines[i].line, "%s", cmd);
        
        if (strcmp(cmd, "push") == 0)
        {
            sscanf((char*)(assm_text->Lines[i].line + 4), "%d", &val);

            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[i].line, "%d %d\n", NAMBER_PUSH, val);

            CPU_text->Lines[i].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[i+1].line = (char*)(CPU_text->Lines[i].line + size_str);
        }

        else if (strcmp(cmd, "add") == 0)
        {
            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[i].line, "%d %d\n", NAMBER_ADD, val);

            CPU_text->Lines[i].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[i+1].line = (char*)(CPU_text->Lines[i].line + size_str);
        }

        else if (strcmp(cmd, "out") == 0)
        {
            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[i].line, "%d %d\n", NAMBER_OUT, val);

            CPU_text->Lines[i].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[i+1].line = (char*)(CPU_text->Lines[i].line + size_str);
        }

        else if (strcmp(cmd, "hlt") == 0)
        {
            size_str = SizeVal(NAMBER_PUSH) + SizeVal(val) + 2;

            sprintf(CPU_text->Lines[i].line, "%d %d\n", NAMBER_HLT, val);

            CPU_text->Lines[i].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[i+1].line = (char*)(CPU_text->Lines[i].line + size_str);
        }

        else 
        {
            printf("unknown command");
            abort();
        }
    }
}