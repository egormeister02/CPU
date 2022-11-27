#include<conio.h>
#include "asm.h"

void CreateAsm(asmtok* assm, FILE* file)
{
    ASSERT(assm != NULL);
    ASSERT(file != NULL);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    assm->buf = (char*)calloc(size + 1, sizeof(char));
    ASSERT(assm->buf != NULL);

    assm->size = fread(assm->buf, sizeof(char), size, file);
    assm->Toks = (Token*)calloc(size / 3, sizeof(Token));
    ASSERT(assm->Toks != NULL);

    size_t* lines = (size_t*)calloc(assm->size, sizeof(size_t));
    ASSERT(lines != NULL);

    size_t nlines = 0;
    for (size_t i = 0; i < assm->size; i++)
    {
        if (assm->buf[i] == '\n')
            lines[nlines++] = i;
    }

    size_t i = 0;
    size_t line = 1;
    assm->Toks[0].str = strtok(assm->buf, " \n");
    while (assm->Toks[i].str != NULL)
    {
        assm->Toks[i].line = line;
        i++;
        assm->Toks[i].str = strtok(NULL, " \n");

        if (assm->Toks[i].str > assm->buf + lines[line-1])
            line++;        
    }

    assm->nTok = i;
    free(lines);
    CreateToks(assm);
}

void CreateToks(asmtok* assm)
{
    ASSERT(assm != NULL);

    for (size_t i = 0; i < assm->nTok; i++)
    {
        size_t dval = POISON_VAL;
        if (IsMem(assm->Toks[i].str))
        {
            assm->Toks[i].type = MEM;
            dval = ScanMem(assm->Toks[i].str);

            if (dval == POISON_VAL)
            {
                printf("invalid memory index\n"
                        "line: %llu", assm->Toks[i].line);
                abort();
            }

            assm->Toks[i].dval = dval;
        }

        else if (IsReg(assm->Toks[i].str))
        {
            assm->Toks[i].type = REG;
            assm->Toks[i].dval = IsReg(assm->Toks[i].str) - 1;
        }

        else if ((size_t)ScanVal(assm->Toks[i].str) != POISON_VAL)
        {
            assm->Toks[i].type = NUM;
            assm->Toks[i].val = ScanVal(assm->Toks[i].str);
        }

        else if (stricmp(assm->Toks[i].str, "push") == 0)
        {
            assm->Toks[i].type = PUSH;
        }

        else if (stricmp(assm->Toks[i].str, "pop") == 0)
        {
            assm->Toks[i].type = POP;
        }

        else if (stricmp(assm->Toks[i].str, "add") == 0)
        {
            assm->Toks[i].type = ADD;
        }

        else if (stricmp(assm->Toks[i].str, "sub") == 0)
        {
            assm->Toks[i].type = SUB;
        }

        else if (stricmp(assm->Toks[i].str, "mul") == 0)
        {
            assm->Toks[i].type = MUL;
        }

        else if (stricmp(assm->Toks[i].str, "div") == 0)
        {
            assm->Toks[i].type = DIV;
        }

        else if (stricmp(assm->Toks[i].str, "in") == 0)
        {
            assm->Toks[i].type = IN;
        }

        else if (stricmp(assm->Toks[i].str, "out") == 0)
        {
            assm->Toks[i].type = OUT;
        }

        else if (stricmp(assm->Toks[i].str, "hlt") == 0)
        {
            assm->Toks[i].type = HLT;
        }

        else 
        {
            printf("unknown token\n"
                   "line: %llu", assm->Toks[i].line);
            abort();
        }
    }
    
}

void CodeCPUCtor(const asmtok* assm, CodeCPU* CPU_code)
{
    ASSERT(assm != NULL);
    ASSERT(CPU_code != NULL);

    CPU_code->signature = CPU_SIGNATURE;
    CPU_code->version = CPU_VERSION;
    CPU_code->nCmd = assm->nTok;
    CPU_code->bin_buf = (void*)calloc(CPU_code->nCmd * 2, sizeof(size_t));

    ASSERT(CPU_code->bin_buf != NULL);
}

void CreateCPUbuf(const asmtok* assm, CodeCPU* CPU_code)
{
    ASSERT(assm != NULL);
    ASSERT(CPU_code != NULL);

    CodeCPUCtor(assm, CPU_code);
    size_t ip = 0;
    int count_cmd = 0;
    for (size_t i = 0; i < assm->nTok; i++)
    {   
        switch (assm->Toks[i].type)
        {
        case PUSH:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[i].type;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            i++;
            if (assm->Toks[i].type == MEM)
            {
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[i].dval;
            }
            else if (assm->Toks[i].type == REG)
            {
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[i].dval;
            }
            else if (assm->Toks[i].type == NUM)
            {
                *((double*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[i].val;
            }
            else 
            {
                printf("invalid push argument\n"
                       "line: %llu", assm->Toks[i].line);
                abort();
            }
            ip++;
            break;

        case POP:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[i].type;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            i++;
            if (assm->Toks[i].type == MEM)
            {
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[i].dval;
            }
            else if (assm->Toks[i].type == REG)
            {
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[i].dval;
            }
            else if (assm->Toks[i].type == NUM)
            {
                printf("invalid pop argument\n"
                       "line: %llu", assm->Toks[i].line);
                abort();
            }
            else 
            {
                i--;
                *ARG_BYIT(CPU_code->bin_buf, ip) = 0;
            }
            ip++;
            break;
        
        default:
            if (assm->Toks[i].type <= MAX_CODE_CMD)
            {
                *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[i].type;
                ip++;
            }
            else
            {
                printf("Command was expected, but an argument was entered\n"
                       "line: %llu", assm->Toks[i].line);
                abort();
            }
            break;
        }
    }
    CPU_code->nCmd = ip;
    
}

void WriteCodeFile(const CodeCPU* CPU_code, FILE* file)
{
    ASSERT(CPU_code != NULL);
    ASSERT(file != NULL);

    void* head_buf = (void*)calloc(2, sizeof(size_t));
    *(int*)head_buf = CPU_code->signature;
    *(int*)((int*)head_buf + 1) = CPU_code->version;
    *(size_t*)((size_t*)head_buf + 1) = CPU_code->nCmd;

    fwrite(head_buf, sizeof(size_t), 2, file);
    fwrite(CPU_code->bin_buf, CPU_code->nCmd * 2, sizeof(size_t), file);
}

int IsMem(const char* sval)
{
    ASSERT(sval != NULL);
    
    if ((sval[0] == '[') && (sval[strlen(sval) - 1] == ']'))
    {
        return 1;
    }
    return 0;
}

size_t ScanMem(const char* sval)
{
    size_t dval = 0;
    for (size_t i = 1; i < strlen(sval) - 1; i++)
    {
        if (sval[i] >= '0' && sval[i] <= '9')
        {
            dval *= 10;
            dval += (sval[i] - '0');
        }
        else return POISON_VAL;
    }
    return dval;
}

double ScanVal(const char* sval)
{
    double val = 0;
    size_t len = strlen(sval);
    size_t raz = len;
    for (size_t i = 0; i < len; i++)
    {
        if (sval[i] >= '0' && sval[i] <= '9')
        {
            val *= 10;
            val += (sval[i] - '0');
        }
        else if (sval[i] == '.' && raz == len)
            raz = i + 1;
        else return POISON_VAL;
    }
    val = val / (pow(10, len - raz));
    return val;
}

size_t IsReg(const char* reg)
{
    ASSERT(reg != NULL);

    if (strlen(reg) == 3 && reg[0] == 'R' && reg[2] == 'X' && reg[1] >= 'A' && reg[1] <= 'P')
        return reg[1] - 'A' + 1;

    return 0;
}