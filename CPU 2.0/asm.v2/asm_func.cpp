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
    for (size_t index = 0; index < assm->size; index++)
    {
        if (assm->buf[index] == '\n')
            lines[nlines++] = index;
    }

    size_t tokcount = 0;
    size_t line = 1;
    assm->Toks[0].str = strtok(assm->buf, " \n");
    while (assm->Toks[tokcount].str != NULL)
    {
        assm->Toks[tokcount].line = line;
        tokcount++;
        assm->Toks[tokcount].str = strtok(NULL, " \n");

        if (assm->Toks[tokcount].str > assm->buf + lines[line-1])
            line++;        
    }

    assm->nTok = tokcount;
    free(lines);
    CreateToks(assm);
}

void CreateToks(asmtok* assm)
{
    ASSERT(assm != NULL);

    for (size_t index = 0; index < assm->nTok; index++)
    {
        size_t dval = POISON_VAL;
        if (IsMem(assm->Toks[index].str))
        {
            assm->Toks[index].type = MEM;
            dval = ScanMem(assm->Toks[index].str);

            if (dval == POISON_VAL)
            {
                printf("ERROR: invalid memory index\n"
                        "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = mem;
                assm->error++;
            }

            assm->Toks[index].dval = dval;
        }

        else if (IsReg(assm->Toks[index].str))
        {
            assm->Toks[index].type = REG;
            assm->Toks[index].dval = IsReg(assm->Toks[index].str) - 1;
        }

        else if (IsJmpArg(assm->Toks[index].str))
        {
            assm->Toks[index].type = JMA;
        }

        else if (IsJmpLin(assm->Toks[index].str))
        {
            assm->Toks[index].type = JML;
            assm->nJLin++;
        }

        else if ((size_t)ScanVal(assm->Toks[index].str) != POISON_VAL)
        {
            assm->Toks[index].type = NUM;
            assm->Toks[index].val = ScanVal(assm->Toks[index].str);
        }

        else if (stricmp(assm->Toks[index].str, "push") == 0)
        {
            assm->Toks[index].type = PUSH;
        }

        else if (stricmp(assm->Toks[index].str, "pop") == 0)
        {
            assm->Toks[index].type = POP;
        }

        else if (stricmp(assm->Toks[index].str, "add") == 0)
        {
            assm->Toks[index].type = ADD;
        }

        else if (stricmp(assm->Toks[index].str, "sub") == 0)
        {
            assm->Toks[index].type = SUB;
        }

        else if (stricmp(assm->Toks[index].str, "mul") == 0)
        {
            assm->Toks[index].type = MUL;
        }

        else if (stricmp(assm->Toks[index].str, "div") == 0)
        {
            assm->Toks[index].type = DIV;
        }

        else if (stricmp(assm->Toks[index].str, "in") == 0)
        {
            assm->Toks[index].type = IN;
        }

        else if (stricmp(assm->Toks[index].str, "out") == 0)
        {
            assm->Toks[index].type = OUT;
        }

        else if (stricmp(assm->Toks[index].str, "hlt") == 0)
        {
            assm->Toks[index].type = HLT;
        }

        else if (stricmp(assm->Toks[index].str, "jmp") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JUN;
            assm->nJmp++;
        }

        else if (stricmp(assm->Toks[index].str, "jb") == 0)
        {
            assm->Toks[index].type  = JMP;
            assm->Toks[index].jtype =  JB;
            assm->nJmp++;
        }

        else if (stricmp(assm->Toks[index].str, "ja") == 0)
        { 
            assm->Toks[index].type  = JMP;
            assm->Toks[index].jtype =  JA;
            assm->nJmp++;
        }

        else if (stricmp(assm->Toks[index].str, "jbe") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JBE;
            assm->nJmp++;
        }

        else if (stricmp(assm->Toks[index].str, "jae") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JAE;
            assm->nJmp++;
        }

        else if (stricmp(assm->Toks[index].str, "jee") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JEE;
            assm->nJmp++;
        }

        else if (stricmp(assm->Toks[index].str, "jne") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JNE;
            assm->nJmp++;
        }

        else 
        {
            printf("ERROR: unknown token\n"
                   "line: %llu\n", assm->Toks[index].line);
            assm->Toks[index].error = tok;
            assm->error++;
        }
        //printf("tok: %s; code: %d; val: %llu\n", assm->Toks[index].str, assm->Toks[index].type, assm->Toks[index].dval);
    }
    
}

void CodeCPUCtor(const asmtok* assm, CodeCPU* CPU_code)
{
    ASSERT(assm != NULL);
    ASSERT(CPU_code != NULL);

    CPU_code->signature = CPU_SIGNATURE;
    CPU_code->version   = CPU_VERSION;
    CPU_code->nCmd      = assm->nTok;
    CPU_code->bin_buf   = (void*)calloc(CPU_code->nCmd * 2, sizeof(size_t));

    ASSERT(CPU_code->bin_buf != NULL);
}

void CreateCPUbuf(const asmtok* assm, CodeCPU* CPU_code)
{
    ASSERT(assm != NULL);
    ASSERT(CPU_code != NULL);

    CodeCPUCtor(assm, CPU_code);

    CPU_code->error = assm->error;

    size_t ip        = 0;
    size_t jcount    = 0;
    size_t jLincount = 0;
    int    flag      = 0;
    int    codetok   = 0;
    double val       = 0;
    int    mem       = 0;
    int    reg       = 0;
    int    dec       = 4;

    jump* jmpLin = (jump*)calloc(assm->nJLin, sizeof(jump));
    jump* jmpCmd = (jump*)calloc(assm->nJmp, sizeof(jump));

    ASSERT(jmpCmd != NULL);
    ASSERT(jmpLin != NULL);

    for (size_t index = 0; index < assm->nTok; index++)
    {   
        codetok = (assm->Toks[index].type == JMP ? assm->Toks[index].jtype: assm->Toks[index].type);
        mem = 0;
        reg = 0;
        dec = 2;
        val = 0;

        fprintf(ListFile, "   %04d   |  %3d   |   %-7s |  %-9d  |  %d  |  %d  |  %4u  |  ",
                            index, assm->Toks[index].line, assm->Toks[index].str, codetok, mem, reg, ip*16 + 16);

        switch (assm->Toks[index].type)
        {
        case PUSH:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].type;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            PrintErr(assm->Toks[index].error);
            index++;
            if (assm->Toks[index].type == MEM)
            {
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                mem = 1;
                dec = 0;
                val = assm->Toks[index].dval;
            }
            else if (assm->Toks[index].type == REG)
            {
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                reg = 1;
                dec = 0;
                val = assm->Toks[index].dval;
            }
            else if (assm->Toks[index].type == NUM)
            {
                *((double*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].val;
                val = assm->Toks[index].val;
            }
            else 
            {
                printf("ERROR: invalid push argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = push_arg;
                CPU_code->error++;
            }

            fprintf(ListFile, "   %04d   |  %3d   |   %-7s |  %-10.*lf |  %d  |  %d  |  %4u  |  ",
                            index, assm->Toks[index].line, assm->Toks[index].str, dec,  val, mem, reg, ip*16 + 24);
            PrintErr(assm->Toks[index].error);

            ip++;
            continue;
            break;

        case POP:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].type;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            PrintErr(assm->Toks[index].error);
            index++;
            if (assm->Toks[index].type == MEM)
            {
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                mem = 1;
                dec = 0;
                val = assm->Toks[index].dval;
            }
            else if (assm->Toks[index].type == REG)
            {
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                reg = 1;
                dec = 0;
                val = assm->Toks[index].dval;
            }
            else if (assm->Toks[index].type == NUM)
            {
                printf("ERROR: invalid pop argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = pop_arg;
                val = assm->Toks[index].val;
                CPU_code->error++;
            }
            else 
            {
                index--;
                *ARG_BYIT(CPU_code->bin_buf, ip) = 0;
                ip++;
                break;
            }

            fprintf(ListFile, "   %04d   |  %3d   |   %-7s |  %-10.*lf |  %d  |  %d  |  %4u  |  ",
                            index, assm->Toks[index].line, assm->Toks[index].str, dec,  val, mem, reg, ip*16 + 24);
            PrintErr(assm->Toks[index].error);

            ip++;
            continue;
            break;

        case JMP:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].jtype;
            *JMP_BYIT(CPU_code->bin_buf, ip) = 1;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            index++;
            if (assm->Toks[index].type == JMA)
            {
                jmpCmd[jcount].ip = ip;
                jmpCmd[jcount].arg = assm->Toks[index].str + 1;
                jmpCmd[jcount].line = assm->Toks[index].line;
                //printf("jcmd[%llu].ip = %llu\n", jcount, jmpCmd[jcount].ip);
                jcount++;
            }
            else 
            {
                printf("ERROR: invalid jump argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = jmp_arg;
                CPU_code->error++;
            }
            ip++;
            break;
        
        case JML:
            
            jmpLin[jLincount].arg  =  assm->Toks[index].str;            
            jmpLin[jLincount].line = assm->Toks[index].line;
            jmpLin[jLincount].ip   =                     ip;

            jmpLin[jLincount].arg[strlen(jmpLin[jLincount].arg) - 1] = 0;
            
            jLincount++;
            //printf("jlin.ip = %llu\n", jmpLin[jLincount - 1].ip);
            break;
        
        default:
            if (assm->Toks[index].type <= MAX_TYPE_TOK)
            {
                *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].type;
                ip++;
            }
            else
            {
                printf("ERROR: command was expected, but an argument was entered\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = arg_cmd;
                CPU_code->error++;
            }
            break;
        }
        PrintErr(assm->Toks[index].error);
    }
    //printf("jmp: %llu\njlin: %llu\n", jcount, jLincount);
    
    for (size_t Jindex = 0; Jindex < jcount; Jindex++)
    {
        flag = 0;
        for (size_t JLinindex = 0; JLinindex < jLincount; JLinindex++)
        {
            //printf("jcmd[%llu].ip = %llu jlin[%llu].ip = %llu\n", Jindex, jmpCmd[Jindex].ip, JLinindex, jmpLin[JLinindex].ip);
            //printf("%s %s\n", jmpCmd[Jindex].arg, jmpLin[JLinindex].arg);
            //printf("%d\n", strcmp(jmpCmd[Jindex].arg, jmpLin[JLinindex].arg));
            if (strcmp(jmpCmd[Jindex].arg, jmpLin[JLinindex].arg) == 0)
            {   
                //printf("OK\n");
                
                if (!flag)
                {
                    
                    *((size_t*)VAL_BYIT(CPU_code->bin_buf, jmpCmd[Jindex].ip)) = jmpLin[JLinindex].ip;
                    flag = 1;
                    //printf("OK\n");
                }
                else
                {
                    printf("ERROR: this jump link has already been met\n"
                           "line: %llu\n", jmpLin[JLinindex].line);
                    CPU_code->error++;
                }
                
            }
        }
    }
    //printf("ERRORS: %d\n", CPU_code->error);
    CPU_code->nCmd = ip;
    
    free(jmpCmd);
    free(jmpLin);
}

void WriteCodeFile(const CodeCPU* CPU_code, FILE* file)
{
    ASSERT(CPU_code != NULL);
    ASSERT(file != NULL);

    void* head_buf = (void*)calloc(2, sizeof(size_t));
    *(int*)head_buf = CPU_code->signature;
    *(int*)((int*)head_buf + 1) = CPU_code->version;
    *(size_t*)((size_t*)head_buf + 1) = CPU_code->nCmd;
    if (CPU_code->error == 0)
    {
        fwrite(head_buf, sizeof(size_t), 2, file);
        fwrite(CPU_code->bin_buf, CPU_code->nCmd * 2, sizeof(size_t), file);
    }
}

void PrintErr(err error)
{
    switch (error)
    {
    case ok:
        fprintf(ListFile, "OK\n");
        break;

    case mem:
        fprintf(ListFile, "ERROR: invalid memory index\n");
        break;

    case tok:
        fprintf(ListFile, "ERROR: unknown token\n");
        break;

    case push_arg:
        fprintf(ListFile, "ERROR: invalid push argument\n");
        break;

    case pop_arg:
        fprintf(ListFile, "ERROR: invalid pop argument\n");
        break;
    
    case jmp_arg:
        fprintf(ListFile, "ERROR: invalid jump argument\n");
        break;

    case arg_cmd:
        fprintf(ListFile, "ERROR: command was expected, but an argument was entered\n");
        break;

    case jmp_link:
        fprintf(ListFile, "ERROR: this jump link has already been met\n");
        break;
    
    default:
        break;
    }
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

size_t IsJmpArg(const char* sval)
{
    ASSERT(sval != NULL);
    if (sval[0] == ':')
        return 1;
    
    else return 0;
}

size_t IsJmpLin(const char* sval)
{
    ASSERT(sval != NULL);

    if (sval[strlen(sval) - 1] == ':')
        return 1;
    
    else return 0;
}

size_t ScanMem(const char* sval)
{
    ASSERT(sval != NULL);

    size_t dval = 0;
    for (size_t index = 1; index < strlen(sval) - 1; index++)
    {
        if (sval[index] >= '0' && sval[index] <= '9')
        {
            dval *= 10;
            dval += (sval[index] - '0');
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
    for (size_t index = 0; index < len; index++)
    {
        if (sval[index] >= '0' && sval[index] <= '9')
        {
            val *= 10;
            val += (sval[index] - '0');
        }
        else if (sval[index] == '.' && raz == len)
            raz = index + 1;
        else return POISON_VAL;
    }
    val = val / (pow(10, len - raz));
    return val;
}

size_t IsReg(const char* reg)
{
    ASSERT(reg != NULL);

    if (strlen(reg) == 3 && reg[0] == 'R' && reg[2] == 'X' && reg[1] >= 'A' && reg[1] <= (char)('A' + SIZE_REG - 1))
        return reg[1] - 'A' + 1;

    return 0;
}