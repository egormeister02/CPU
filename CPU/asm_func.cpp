#include<conio.h>
#include "asm.h"

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
    CPU_code->nCmd = assm_text->nlines;
    CPU_code->bin_buf = (void*)calloc(CPU_code->nCmd * 2, sizeof(size_t));

    ASSERT(CPU_code->bin_buf != NULL);
}

void CreateCPUbuf(const TEXT* assm_text, CodeCPU* CPU_code)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_code != NULL);

    CodeCPUCtor(assm_text, CPU_code);
    char*  svalbuf = NULL;
    char*  sval = NULL;

    svalbuf = (char*)calloc(MAX_LENGTH_VAL, sizeof(char));
    ASSERT(svalbuf != NULL);

    int count_cmd = 0;
    for (int i = 0; i < (long)assm_text->nlines; i++)
    {   
        char   cmd[MAX_SIZE_COMMAND] =         {};
        double val                   = POISON_VAL;
        int nch = 0;

        sval = svalbuf;

        sscanf(assm_text->Lines[i].line, " %s %n", cmd, &nch);
        if (stricmp(cmd, "push") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_PUSH;
            *ARG_BYIT(CPU_code->bin_buf, count_cmd) = 1;
            sscanf((char*)(assm_text->Lines[i].line + nch), " %s", sval);

            if (sval != IsMem(sval))
            {
                sval += 1;
                *MEM_BYIT(CPU_code->bin_buf, count_cmd) = 1;
            }           
            if (IsReg(sval))
            {
                *REG_BYIT(CPU_code->bin_buf, count_cmd) = 1; 
                *(size_t*)(VAL_BYIT(CPU_code->bin_buf, count_cmd)) = IsReg(sval) - 1;
            }
            else 
            {
                sscanf(sval, "%lf", &val);
                if ((long)val == POISON_VAL) 
                {
                    printf("invalid number format\n"
                            "line: %d", i+1);
                    abort();
                }
                *((double*)VAL_BYIT(CPU_code->bin_buf, count_cmd)) = val;
            }

            count_cmd ++;
        }

        else if (stricmp(cmd, "add") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_ADD;
            count_cmd ++;
        }

        else if (stricmp(cmd, "sub") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_SUB;
            count_cmd ++;
        }

        else if (stricmp(cmd, "mul") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_MUL;
            count_cmd ++;
        }

        else if (stricmp(cmd, "div") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_DIV;
            count_cmd ++;
        }

        else if (stricmp(cmd, "In") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_IN;
            count_cmd ++;
        }

        else if (stricmp(cmd, "pop") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_POP;
            count_cmd ++;
        }

        else if (stricmp(cmd, "out") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_OUT;
            count_cmd ++;
        }

        else if (stricmp(cmd, "hlt") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_HLT;
            count_cmd ++;
        }
        else 
        {
            printf("unknown command\n"
                   "line: %d", i+1);
            abort();
        }
    }
    CPU_code->nCmd = count_cmd;
    free(svalbuf);
}

void WriteCodeFile(const CodeCPU* CPU_code, FILE* file)
{
    void* head_buf = (void*)calloc(2, sizeof(size_t));
    *(int*)head_buf = CPU_code->signature;
    *(int*)((int*)head_buf + 1) = CPU_code->version;
    *(size_t*)((size_t*)head_buf + 1) = CPU_code->nCmd;

    fwrite(head_buf, sizeof(size_t), 2, file);
    fwrite(CPU_code->bin_buf, CPU_code->nCmd * 2, sizeof(size_t), file);
}

char* IsMem(char* sval)
{
    if ((sval[0] == '[') && (sval[strlen(sval) - 1] == ']'))
    {
        sval[strlen(sval) - 1] = 0;
        return sval + 1;
    }
    return sval;
}

size_t IsReg(const char* reg)
{
    ASSERT(reg != NULL);

    if (strlen(reg) == 3 && reg[0] == 'R' && reg[2] == 'X' && reg[1] >= 'A' && reg[1] <= 'P')
        return reg[1] - 'A' + 1;

    return 0;
}
