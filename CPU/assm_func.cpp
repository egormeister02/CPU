#include<conio.h>
#include "assm.h"

void CreateCPUtext(const TEXT* assm_text, TEXT* CPU_text)
{    
    ASSERT(assm_text != NULL);
    ASSERT(CPU_text != NULL);

    CPU_text->buf = (char*)calloc((assm_text->nlines + 1) * MAX_CPU_STR, sizeof(char));
    ASSERT(CPU_text->buf != NULL);

    CPU_text->nlines = assm_text->nlines + 1;

    CPU_text->Lines = (LINE*)calloc(CPU_text->nlines, sizeof(LINE));
    ASSERT(CPU_text->Lines != NULL);

    CPU_text->Lines[0].line = CPU_text->buf;
}

void CodeCPUCtor(const TEXT* assm_text, CodeCPU* CPU_code)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_code != NULL);

    CPU_code->signature = CPU_SIGNATURE;
    CPU_code->version = CPU_VERSION;
    CPU_code->nElem = assm_text->nlines * 2;
    CPU_code->bin_buf = (void*)calloc(CPU_code->nElem, sizeof(size_t));

    ASSERT(CPU_code->bin_buf != NULL);
}

void CreateCPUbuf(const TEXT* assm_text, CodeCPU* CPU_code)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_code != NULL);

    CodeCPUCtor(assm_text, CPU_code);

    int index = 0;
    for (int i = 0; i < (long)assm_text->nlines; i++)
    {   
        char   cmd[MAX_SIZE_COMMAND] =         "";
        double val                   = POISON_VAL;
        //char   sval[MAX_LENGTH_VAL]  =         "";
        //size_t size_str              =          0;
        int nch = 0;

        sscanf(assm_text->Lines[i].line, " %s %n", cmd, &nch);

        if (stricmp(cmd, "push") == 0)
        {
            sscanf((char*)(assm_text->Lines[i].line + nch), "%lf", &val);
            if ((long)val == POISON_VAL) 
            {
                printf("invalid number format\n"
                        "line: %d", i+1);
                abort();
            }
            *(size_t*)((size_t*)CPU_code->bin_buf + (index++)) = CMD_PUSH;
            *(double*)((double*)CPU_code->bin_buf + (index++)) =      val;
        }

        else if (stricmp(cmd, "add") == 0)
        {
            *(size_t*)((size_t*)CPU_code->bin_buf + (index++)) =  CMD_ADD;
            *(double*)((double*)CPU_code->bin_buf + (index++)) =  NAN_VAL;
        }

        else if (stricmp(cmd, "out") == 0)
        {
            *(size_t*)((size_t*)CPU_code->bin_buf + (index++)) =  CMD_OUT;
            *(double*)((double*)CPU_code->bin_buf + (index++)) =  NAN_VAL;
        }

        else if (stricmp(cmd, "hlt") == 0)
        {
            *(size_t*)((size_t*)CPU_code->bin_buf + (index++)) =  CMD_HLT;
            *(double*)((double*)CPU_code->bin_buf + (index++)) =  NAN_VAL;
        }

        else 
        {
            printf("unknown command\n"
                   "line: %d", i+1);
            abort();
        }
    }
    CPU_code->nElem = index;
}
/*
void CreateCodeFile(const TEXT* CPU_text, FILE* codefile)
{
    size_t size = 0;
    size = 
}


size_t SizeVal(int val)
{
    size_t size_val = 0;
    do
    {
        val /= 10;
        size_val++;
    } while (abs(val) > 0);

    return size_val;
}
*/
