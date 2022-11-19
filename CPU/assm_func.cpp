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
    CPU_code->nCmd = assm_text->nlines;
    CPU_code->bin_buf = (void*)calloc(CPU_code->nCmd * 2, sizeof(size_t));

    ASSERT(CPU_code->bin_buf != NULL);
}

void CreateCPUbuf(const TEXT* assm_text, CodeCPU* CPU_code)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_code != NULL);

    CodeCPUCtor(assm_text, CPU_code);
    

    int count_cmd = 0;
    for (int i = 0; i < (long)assm_text->nlines; i++)
    {   
        printf("ncmd = %d\n", count_cmd);
        char   cmd[MAX_SIZE_COMMAND] =         {};
        double val                   = POISON_VAL;
        int nch = 0;
        char*  svalbuf = NULL;
        char*  sval = NULL;

        svalbuf = (char*)calloc(MAX_LENGTH_VAL, sizeof(char));
        ASSERT(svalbuf != NULL);
        sval = svalbuf;

        sscanf(assm_text->Lines[i].line, " %s %n", cmd, &nch);
        printf("%s \n", cmd);
        if (stricmp(cmd, "push") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_PUSH;
            *ARG_BYIT(CPU_code->bin_buf, count_cmd) = 1;
            sscanf((char*)(assm_text->Lines[i].line + nch), " %s", sval);
            printf("%s\n", sval);

            if (sval != IsMem(sval))
            {
                sval += 1;
                *MEM_BYIT(CPU_code->bin_buf, count_cmd) = 1;
                printf("%s\n", sval);
            }
            
            if (IsReg(sval))
            {
                *REG_BYIT(CPU_code->bin_buf, count_cmd) = 1; 
                *(size_t*)(VAL_BYIT(CPU_code->bin_buf, count_cmd)) = IsReg(sval) - 1;
                printf("OK");
            }
            else 
            {
                sscanf(sval, "%lf", &val);
                printf("val = %lf\n", val);
                if ((long)val == POISON_VAL) 
                {
                    printf("invalid number format\n"
                            "line: %d", i+1);
                    abort();
                }
                *((double*)VAL_BYIT(CPU_code->bin_buf, count_cmd)) = val;
                printf("OK\n");
            }
            count_cmd ++;
            //free(svalbuf);
            printf("OK\n");
        }

        else if (stricmp(cmd, "add") == 0)
        {
            *CMD_BYIT(CPU_code->bin_buf, count_cmd) = CMD_ADD;
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
        printf("%d\n", count_cmd);
        
        
        printf("OK\n");
    }
    CPU_code->nCmd = count_cmd;
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
