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

void CreateCPUcode(const TEXT* assm_text, TEXT* CPU_text)
{
    ASSERT(assm_text != NULL);
    ASSERT(CPU_text != NULL);
    CreateCPUtext(assm_text, CPU_text);
    int j = 0;
    for (int i = 0; i < (long)assm_text->nlines; i++)
    {   
        char   cmd[MAX_SIZE_COMMAND] =         "";
        double val                   = POISON_VAL;
        char   sval[MAX_LENGTH_VAL]  =         "";
        size_t size_str              =          0;

        sscanf(assm_text->Lines[i].line, "%s", cmd);

        if (strcmp(cmd, "") == 0) continue;
        else if (stricmp(cmd, "push") == 0)
        {
            sscanf((char*)(assm_text->Lines[i].line + 4), "%lf", &val);
            if (val == POISON_VAL) 
            {
                printf("invalid number format\n"
                        "line: %d", i+1);
                abort();
            }
            sscanf((char*)(assm_text->Lines[i].line + 4), "%s", sval);
            size_str = SizeVal(CMD_PUSH) + strlen(sval) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %s\n", CMD_PUSH, sval);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else if (stricmp(cmd, "add") == 0)
        {
            size_str = SizeVal(CMD_ADD) + SizeVal(NAN_VAL) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", CMD_ADD, NAN_VAL);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else if (stricmp(cmd, "out") == 0)
        {
            size_str = SizeVal(CMD_OUT) + SizeVal(NAN_VAL) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", CMD_OUT, NAN_VAL);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else if (stricmp(cmd, "hlt") == 0)
        {
            size_str = SizeVal(CMD_HLT) + SizeVal(NAN_VAL) + 2;

            sprintf(CPU_text->Lines[j].line, "%d %d\n", CMD_HLT, NAN_VAL);

            CPU_text->Lines[j].length = size_str;
            CPU_text->size += size_str;
            CPU_text->Lines[j+1].line = (char*)(CPU_text->Lines[j].line + size_str);
            j++;
        }

        else 
        {
            printf("unknown command\n"
                   "line: %d", i+1);
            abort();
        }
    }
    CPU_text->nlines = j + 1;
}


void TextDumpFunc(const TEXT* text, FILE* LogFile)
{   
    ASSERT(text != NULL);

    fprintf(LogFile, "\n----------------------------------TextDump----------------------------------\n");
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