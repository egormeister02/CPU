#include "CPU.h"

void ReadHead( CodeCPU* CPU_code, FILE* codefile)
{
    ASSERT(CPU_code != NULL);
    ASSERT(codefile != NULL);

    void* head = (void*)calloc(2, sizeof(size_t));
    ASSERT(head != NULL);

    fread(head, 2, sizeof(size_t), codefile);
    CPU_code->signature = *((int*)head);
    CPU_code->version = *(int*)(((int*)head) + 1);

    if (CPU_code->signature != CPU_SIGNATURE)
    {
        printf("ERROR: invalid signature!!!\n");
        CPU_code->nCmd = 0;
    }
    else if (CPU_code->version != CPU_VERSION)
    {
        printf("ERROR: invalid version!!!\n");
        CPU_code->nCmd = 0;
    }
    else 
        CPU_code->nCmd = *((size_t*)((size_t*)head + 1));

    free(head);
}

void CreateCPU(CodeCPU* CPU_code, FILE* codefile)
{
    ASSERT(CPU_code != NULL);
    ASSERT(codefile != NULL);

    ReadHead(CPU_code, codefile);

    CPU_code->bin_buf = (void*)calloc(CPU_code->nCmd * 2, sizeof(size_t));
    ASSERT(CPU_code->bin_buf != NULL);

    CPU_code->ram = (double*)calloc(SIZE_RAM, sizeof(double));
    ASSERT(CPU_code->ram != NULL)

    fread(CPU_code->bin_buf, sizeof(size_t), CPU_code->nCmd * 2, codefile);
}

void DoProgram(CodeCPU* CPU_code)
{
    ASSERT(CPU_code != NULL);
    if (!CPU_code->nCmd) return;

    stk stk1          =    {};
    stk stk2          =    {};
    StackCtor(&stk1, 0)      ;
    StackCtor(&stk2, 0)      ;
    CPU_code->stk     = &stk1;
    CPU_code->ret_stk = &stk2;

    size_t cmd                =                            0;
    void(**command)(CodeCPU*) = CreateArrayCmd(MAX_CODE_CMD);

    while (CPU_code->ip < CPU_code->nCmd)
    {
        cmd = (size_t)(*CMD_BYIT(CPU_code->bin_buf, CPU_code->ip));
        command[cmd](CPU_code);
        //printf("%d\n", CPU_code->ip);
    }

    printf("\nend of process");

    DtorCPU(CPU_code);
    free(command);
}

void DtorCPU(CodeCPU* CPU_code)
{
    ASSERT(CPU_code != NULL);

    StackDtor(CPU_code->stk);
    free(CPU_code->bin_buf);
    free(CPU_code->ram);

    CPU_code->signature  = (size_t)POISON;
    CPU_code->version    = (size_t)POISON;
    CPU_code->nCmd       =        0;
    CPU_code->ip         =        0;
    CPU_code->bin_buf    =     NULL;
    CPU_code->ram        =     NULL;
    CPU_code->stk        =     NULL;
}

void(**CreateArrayCmd(size_t number_cmd))(CodeCPU*) 
{
    void(**CMD)(CodeCPU*);
    CMD = (void(**)(CodeCPU*))calloc(number_cmd + 1, sizeof(void(*)));

    CMD[CMD_PUSH] =  Push_CMD;
    CMD[CMD_ADD]  =   Add_CMD;
    CMD[CMD_OUT]  =   Out_CMD;
    CMD[CMD_SUB]  =   Sub_CMD;
    CMD[CMD_MUL]  =   Mul_CMD;
    CMD[CMD_DIV]  =   Div_CMD;
    CMD[CMD_IN]   =    In_CMD;
    CMD[CMD_POP]  =   Pop_CMD;
    CMD[CMD_HLT]  =   Hlt_CMD;
    CMD[CMD_JMP]  =   Jmp_CMD;
    CMD[CMD_JB]   =    Jb_CMD;
    CMD[CMD_JA]   =    Ja_CMD;
    CMD[CMD_JBE]  =   Jbe_CMD;
    CMD[CMD_JAE]  =   Jae_CMD;
    CMD[CMD_JEE]  =   Jee_CMD;
    CMD[CMD_JNE]  =   Jne_CMD;
    CMD[CMD_CALL] =  Call_CMD;
    CMD[CMD_RET]  =   Ret_CMD;
    CMD[CMD_GRA]  = Graph_CMD;
    return CMD;
}

void Push_CMD(CodeCPU* CPU_code)
{
    double val = 0;
    size_t memid = 0;
    if (*MEM_BYIT(CPU_code->bin_buf, CPU_code->ip))
    {
        if (*REG_BYIT(CPU_code->bin_buf, CPU_code->ip))
        {
            size_t arg = (size_t)*(char*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip) - 1;
            char add = *((char*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip) + 1);
            if (int(arg + add) < 0)
            {
                printf("\n!_exetuation error_! \n\tin Push_CMD(): memory index < 0 \n\texecuted: Push 0");
                Push(CPU_code->stk, 0);
                CPU_code->ip++;
                return;
            }
            else 
                memid = (size_t)CPU_code->reg[arg] + add;
        }
        else
            memid = *(size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip);

        val = CPU_code->ram[memid];
    }
    else if (*REG_BYIT(CPU_code->bin_buf, CPU_code->ip))
    {
        val = (double)CPU_code->reg[*(char*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip) - 1];
    }
    else
        val = *(double*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip);

    Push(CPU_code->stk, val);
    CPU_code->ip++;
}

void Add_CMD(CodeCPU* CPU_code)
{
    Push(CPU_code->stk, Pop(CPU_code->stk) + Pop(CPU_code->stk));
    CPU_code->ip++;
}

void Sub_CMD(CodeCPU* CPU_code)
{
    Push(CPU_code->stk, Pop(CPU_code->stk) - Pop(CPU_code->stk));
    CPU_code->ip++;
}

void Mul_CMD(CodeCPU* CPU_code)
{
    Push(CPU_code->stk, Pop(CPU_code->stk) * Pop(CPU_code->stk));
    CPU_code->ip++;
}

void Div_CMD(CodeCPU* CPU_code)
{
    Push(CPU_code->stk, Pop(CPU_code->stk) / Pop(CPU_code->stk));
    CPU_code->ip++;
}

void Pop_CMD(CodeCPU* CPU_code)
{
    if (*ARG_BYIT(CPU_code->bin_buf, CPU_code->ip))
    {
        size_t memid = 0;
        if (*MEM_BYIT(CPU_code->bin_buf, CPU_code->ip))
        {
            if (*REG_BYIT(CPU_code->bin_buf, CPU_code->ip))
            {
                size_t arg = (size_t)*(char*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip) - 1;
                char add = *((char*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip) + 1);
                if (int(arg + add) < 0)
                {
                    printf("\n!_exetuation error_! \n\tin Push_CMD(): memory index < 0 \n\texecuted: Pop");
                    Pop(CPU_code->stk);
                    CPU_code->ip++;
                    return;
                }
                else 
                    memid = (size_t)CPU_code->reg[arg] + add;
            }
            else
                memid = *(size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip);

            CPU_code->ram[memid] = Pop(CPU_code->stk);
        }
        else if (*REG_BYIT(CPU_code->bin_buf, CPU_code->ip))
        {
             CPU_code->reg[*(char*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip) - 1] = Pop(CPU_code->stk);
        }          
    }
    else 
        Pop(CPU_code->stk);
    CPU_code->ip++;
}

void In_CMD(CodeCPU* CPU_code)
{
    double x = 0;
    printf("\nenter the number\n");
    scanf("%lf", &x);
    Push(CPU_code->stk, x);
    CPU_code->ip++;
}

void Out_CMD(CodeCPU* CPU_code)
{
    printf("%lf\n", Pop(CPU_code->stk));
    CPU_code->ip++;
}

void Hlt_CMD(CodeCPU* CPU_code)
{
    CPU_code->ip = CPU_code->nCmd;
}

void Jmp_CMD(CodeCPU* CPU_code)
{
    CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
}

void Jb_CMD(CodeCPU* CPU_code)
{
    if (Pop(CPU_code->stk) - Pop(CPU_code->stk) < EPSILA)
        CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
    else
        CPU_code->ip++;
}

void Ja_CMD(CodeCPU* CPU_code)
{
    if (Pop(CPU_code->stk) - Pop(CPU_code->stk) > EPSILA)
        CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
    else
        CPU_code->ip++;
}

void Jbe_CMD(CodeCPU* CPU_code)
{
    if (Pop(CPU_code->stk) - Pop(CPU_code->stk) < EPSILA)
        CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
    else
        CPU_code->ip++;
}

void Jae_CMD(CodeCPU* CPU_code)
{
    if (Pop(CPU_code->stk) - Pop(CPU_code->stk) > -EPSILA)
        CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
    else
        CPU_code->ip++;
}

void Jee_CMD(CodeCPU* CPU_code)
{
    if (abs(Pop(CPU_code->stk) - Pop(CPU_code->stk)) < EPSILA)
        CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
    else
        CPU_code->ip++;
}

void Jne_CMD(CodeCPU* CPU_code)
{
    if (abs(Pop(CPU_code->stk) - Pop(CPU_code->stk)) < EPSILA)
        CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
    else
        CPU_code->ip++;
}

void Call_CMD(CodeCPU* CPU_code)
{
    Push(CPU_code->ret_stk, (double)(CPU_code->ip + 1));
    CPU_code->ip = *((size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip));
}

void Ret_CMD(CodeCPU* CPU_code)
{
    CPU_code->ip = (size_t)Pop(CPU_code->ret_stk);
}

void Graph_CMD(CodeCPU* CPU_code)
{
    char* imag = (char*)calloc(SIZE_RAM + GR_RESOL, sizeof(char));
    for (size_t i = 0; i < GR_RESOL; i++)
    {
        for (size_t j = 0; j < GR_RESOL; j++)
        {
            imag[i*(GR_RESOL + 1) + j] = (char)CPU_code->ram[i*GR_RESOL + j];
        }
        imag[(i + 1)*(GR_RESOL + 1) - 1] = '\n';
    }
    puts(imag);
    CPU_code->ip++;
}