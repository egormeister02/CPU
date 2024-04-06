//#include<conio.h>
#include "asm.h"

FILE* ListFile = StartList();

void MakeCodeFileName(const char* assm_filename, char* code_filename)
{
    size_t assm_name_len = strlen(assm_filename);

    // Копируем имя входного файла в выходной файл
    strncpy(code_filename, assm_filename, assm_name_len);

    // Добавляем расширение ".out" к имени выходного файла
    strncpy(code_filename + assm_name_len - 4, ".code\0", 6);
}

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

    size_t* lines = (size_t*)calloc(assm->size, sizeof(size_t));
    ASSERT(lines != NULL);

    size_t nlines = 0;
    for (size_t index = 0; index < assm->size; index++)
    {
        if (assm->buf[index] == '\n')
            lines[nlines++] = index;
    }

    assm->Toks = (Token*)calloc(nlines * 2, sizeof(Token));
    ASSERT(assm->Toks != NULL);

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

        if (IsMrg(assm->Toks[index].str))
        {
            assm->Toks[index].type = MRG;
            assm->Toks[index].dval = IsMrg(assm->Toks[index].str);
        }

        else if (IsMemExp(assm->Toks[index].str))
        {
            assm->Toks[index].type = MRG;
            assm->Toks[index].dval = IsMemExp(assm->Toks[index].str);
        }

        else if (IsMem(assm->Toks[index].str))
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
            assm->Toks[index].dval = IsReg(assm->Toks[index].str);
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

        else if (strcasecmp(assm->Toks[index].str, "push") == 0)
        {
            assm->Toks[index].type = PUSH;
        }

        else if (strcasecmp(assm->Toks[index].str, "pop") == 0)
        {
            assm->Toks[index].type = POP;
        }

        else if (strcasecmp(assm->Toks[index].str, "add") == 0)
        {
            assm->Toks[index].type = ADD;
        }

        else if (strcasecmp(assm->Toks[index].str, "sub") == 0)
        {
            assm->Toks[index].type = SUB;
        }

        else if (strcasecmp(assm->Toks[index].str, "mul") == 0)
        {
            assm->Toks[index].type = MUL;
        }

        else if (strcasecmp(assm->Toks[index].str, "div") == 0)
        {
            assm->Toks[index].type = DIV;
        }

        else if (strcasecmp(assm->Toks[index].str, "in") == 0)
        {
            assm->Toks[index].type = IN;
        }

        else if (strcasecmp(assm->Toks[index].str, "out") == 0)
        {
            assm->Toks[index].type = OUT;
        }

        else if (strcasecmp(assm->Toks[index].str, "ret") == 0)
        {
            assm->Toks[index].type = RET;
        }

        else if (strcasecmp(assm->Toks[index].str, "graph") == 0)
        {
            assm->Toks[index].type = GRA;
        }

        else if (strcasecmp(assm->Toks[index].str, "hlt") == 0)
        {
            assm->Toks[index].type = HLT;
        }

        else if (strcasecmp(assm->Toks[index].str, "jmp") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JUN;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "jb") == 0)
        {
            assm->Toks[index].type  = JMP;
            assm->Toks[index].jtype =  JB;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "ja") == 0)
        { 
            assm->Toks[index].type  = JMP;
            assm->Toks[index].jtype =  JA;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "jbe") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JBE;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "jae") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JAE;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "jee") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JEE;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "jne") == 0)
        {
            assm->Toks[index].type  =  JMP;
            assm->Toks[index].jtype =  JNE;
            assm->nJmp++;
        }

        else if (strcasecmp(assm->Toks[index].str, "call") == 0)
        {
            assm->Toks[index].type =  CALL;
            assm->nJmp++;
        }

        else 
        {
            printf("ERROR: unknown token\n"
                   "line: %llu\n", assm->Toks[index].line);
            assm->Toks[index].error = tok;
            assm->error++;
        }
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
    ASSERT(assm     != NULL);
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
    int    dec       = 0;

    jump* jmpLin = (jump*)calloc(assm->nJLin, sizeof(jump));
    jump* jmpCmd =  (jump*)calloc(assm->nJmp, sizeof(jump));

    ASSERT(jmpCmd != NULL);
    ASSERT(jmpLin != NULL);

    for (size_t index = 0; index < assm->nTok; index++)
    {   
        codetok = (assm->Toks[index].type == JMP ? (int)assm->Toks[index].jtype :
                   assm->Toks[index].type == JML ? (int)(ip*16 + 16) :
                   (int)assm->Toks[index].type);
        mem = 0;
        reg = 0;
        dec = 2;
        val = 0;

        fprintf(ListFile, "   %04llu   |  %3llu   |   %-8s |  %-9d  |  %d  |  %d  | %5llu  |  ",
                            index, assm->Toks[index].line, assm->Toks[index].str, codetok, mem, reg, ip*16 + 16);

        switch (assm->Toks[index].type)
        {
        case PUSH:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].type;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            PrintErr(assm->Toks[index].error);
            index++;
            switch (assm->Toks[index].type)
            {
            case MRG:
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                mem = 1;
                reg = 1;
                dec = 0;
                if (*((char*)&assm->Toks[index].dval + 1) != 0)
                {
                    double add = abs(*((char*)&assm->Toks[index].dval + 1));
                    while (add >= 1)
                    {
                        add /= 10;
                        dec++;
                    }
                    val = *((char*)&assm->Toks[index].dval) + add;
                }
                else
                    val = (double)assm->Toks[index].dval;

                break;

            case MEM:
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                mem = 1;
                dec = 0;
                val = (double)assm->Toks[index].dval;
                break;

            case REG:
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                reg = 1;
                dec = 0;
                val = (double)assm->Toks[index].dval;
                break;

            case NUM:
                *((double*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].val;
                val = assm->Toks[index].val;
                break;
            
            default:
                printf("ERROR: invalid push argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = push_arg;
                CPU_code->error++;
                break;
            }

            fprintf(ListFile, "   %04llu   |  %3llu   |   %-8s |  %-10.*lf |  %d  |  %d  | %5llu  |  ",
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

            switch (assm->Toks[index].type)
            {
            case MRG:
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                mem = 1;
                reg = 1;
                dec = 0;
                if (*((char*)&assm->Toks[index].dval + 1) != 0)
                {
                    double add = abs(*((char*)&assm->Toks[index].dval + 1));
                    while (add >= 1)
                    {
                        add /= 10;
                        dec++;
                    }
                    val = *((char*)&assm->Toks[index].dval) + add;
                }
                else
                    val = (double)assm->Toks[index].dval;
                break;

            case MEM:
                *MEM_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                mem = 1;
                dec = 0;
                val = (double)assm->Toks[index].dval;
                break;

            case REG:
                *REG_BYIT(CPU_code->bin_buf, ip) = 1;
                *((size_t*)VAL_BYIT(CPU_code->bin_buf, ip)) = assm->Toks[index].dval;
                reg = 1;
                dec = 0;
                val = (double)assm->Toks[index].dval;
                break;

            case NUM:
                printf("ERROR: invalid pop argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = pop_arg;
                val = assm->Toks[index].val;
                CPU_code->error++;
                break;
            
            default:
                index--;
                *ARG_BYIT(CPU_code->bin_buf, ip) = 0;
                ip++;
                break;
            }

            fprintf(ListFile, "   %04llu   |  %3llu   |   %-8s |  %-10.*lf |  %d  |  %d  | %5llu  |  ",
                            index, assm->Toks[index].line, assm->Toks[index].str, dec,  val, mem, reg, ip*16 + 24);
            PrintErr(assm->Toks[index].error);

            ip++;
            continue;
            break;

        case JMP:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].jtype;
            *JMP_BYIT(CPU_code->bin_buf, ip) = 1;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            PrintErr(assm->Toks[index].error);
            index++;
            if (assm->Toks[index].type == JMA)
            {
                jmpCmd[jcount].ip = ip;
                jmpCmd[jcount].arg = assm->Toks[index].str + 1;
                jmpCmd[jcount].line = assm->Toks[index].line;
                jmpCmd[jcount].numtok = index;
                jcount++;

            }
            else 
            {
                printf("ERROR: invalid jump argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = jmp_arg;
                CPU_code->error++;
            }

            fprintf(ListFile, "   %04llu   |  %3llu   |   %-8s |  ---------  |  %d  |  %d  | %5llu  |  ",
                              index, assm->Toks[index].line, assm->Toks[index].str, mem, reg, ip*16 + 8);
            PrintErr(assm->Toks[index].error);
            ip++;
            continue;
            break;

        case CALL:
            *CMD_BYIT(CPU_code->bin_buf, ip) = assm->Toks[index].type;
            *CALL_BYIT(CPU_code->bin_buf, ip) = 1;
            *ARG_BYIT(CPU_code->bin_buf, ip) = 1;
            PrintErr(assm->Toks[index].error);
            index++;
            if (assm->Toks[index].type == JMA)
            {
                jmpCmd[jcount].ip = ip;
                jmpCmd[jcount].arg = assm->Toks[index].str + 1;
                jmpCmd[jcount].line = assm->Toks[index].line;
                jmpCmd[jcount].numtok = index;
                jcount++;

            }
            else 
            {
                printf("ERROR: invalid call argument\n"
                       "line: %llu\n", assm->Toks[index].line);
                assm->Toks[index].error = call_arg;
                CPU_code->error++;
            }

            fprintf(ListFile, "   %04llu   |  %3llu   |   %-8s |  ---------  |  %d  |  %d  | %5llu  |  ",
                              index, assm->Toks[index].line, assm->Toks[index].str, mem, reg, ip*16 + 8);
            PrintErr(assm->Toks[index].error);
            ip++;
            continue;
            break;
        
        case JML:
            
            jmpLin[jLincount].arg    =  assm->Toks[index].str;            
            jmpLin[jLincount].line   = assm->Toks[index].line;
            jmpLin[jLincount].ip     =                     ip;
            jmpLin[jLincount].numtok =                  index;

            jmpLin[jLincount].arg[strlen(jmpLin[jLincount].arg) - 1] = 0;
            
            jLincount++;
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
    
    for (size_t Jindex = 0; Jindex < jcount; Jindex++)
    {
        flag = 0;
        for (size_t JLinindex = 0; JLinindex < jLincount; JLinindex++)
        {
            if (strcmp(jmpCmd[Jindex].arg, jmpLin[JLinindex].arg) == 0)
            {   
                if (!flag)
                {
                    
                    *((size_t*)VAL_BYIT(CPU_code->bin_buf, jmpCmd[Jindex].ip)) = jmpLin[JLinindex].ip;
                    flag = 1;
                }
                else
                {
                    printf("ERROR: this jump link has already been met\n"
                           "line: %llu\n", jmpLin[JLinindex].line);
                    fprintf(ListFile, "\nERROR: this jump link has already been met\n\tNumTok: %04llu\n", jmpLin[JLinindex].numtok);
                    CPU_code->error++;
                }
                
            }
        }
        if (!flag)
        {
            printf("ERROR: not found link for this jump or call\n"
                           "line: %llu\n", jmpCmd[Jindex].line);
            fprintf(ListFile, "\nERROR: not found link for this jump or call\n\tNumTok: %04llu\n", jmpCmd[Jindex].numtok);
                    CPU_code->error++;
        }
    }

    CPU_code->nCmd = ip;
    FinishList();

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
        fprintf(ListFile, "ERROR: this link has already been met\n");
        break;

    case call_arg:
        fprintf(ListFile, "ERROR: invalid call argument\n");
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
    int minus = 1;
    if (sval[0] == '-')
    {
        minus = -1;
        sval += 1;
    }

    size_t len = strlen(sval);
    size_t raz = len;
    double val = 0;
    
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
    return val * minus;
}

size_t IsReg(const char* reg)
{
    ASSERT(reg != NULL);

    if (strlen(reg) == 3 && reg[0] == 'R' && reg[2] == 'X' && reg[1] >= 'A' && reg[1] <= (char)('A' + SIZE_REG - 1))
        return reg[1] - 'A' + 1;

    return 0;
}

size_t IsMrg(const char* sval)
{
    size_t result = 0;
    if ((sval[0] == '[') && (sval[strlen(sval) - 1] == ']'))
    {
        if (strlen(sval) == 5 && sval[1] == 'R' && sval[3] == 'X' && sval[2] >= 'A' && sval[2] <= (char)('A' + SIZE_REG - 1))
        {
            *(char*)&result = sval[2] - 'A' + 1;
        }
        return result;
    }

    return 0;
}

size_t IsMemExp(const char* sval)
{
    char   vals[3]  = {};
    char   val      =  0;
    char   arg      =  0;
    char   add      =  0;
    size_t result   =  0;

    if (IsMem(sval) && (strlen(sval) > 5) && (sval[1] == 'R' && sval[3] == 'X' && sval[2] >= 'A' && sval[2] <= (char)('A' + SIZE_REG - 1)))
    {
        arg = sval[2] - 'A' + 1;

        for (size_t i = 5; i < strlen(sval) - 1; i++)
            vals[i-5] = sval[i];

        if ((size_t)ScanVal(vals) != POISON_VAL)
        {
            val = (char)ScanVal(vals);
            if (sval[4] == '-')
                add = val * (-1);

            else if (sval[4] == '+')
                add = val;

            else return 0;
        }
        else return 0;

        *(char*)&result = arg;
        *((char*)&result + 1) = add;
        return result;
    }
    else return 0;
}

FILE* StartList(void)
{
    ListFile = fopen(LIST_FILE, "w");
    fprintf(ListFile, "-------------------------------Start_Listing-------------------------------\n\n");
    fprintf(ListFile, "  NumTok  |  Line  |  String    |  code/val   | MEM | REG |   pos  |  comment  \n");
    return ListFile;
}

void FinishList(void)
{
    fprintf(ListFile, "\n-------------------------------Finish_Listing------------------------------\n");
    fclose(ListFile);
}