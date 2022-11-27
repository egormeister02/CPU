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
        abort();
    }
    if (CPU_code->version != CPU_VERSION)
    {
        printf("ERROR: invalid version!!!\n");
        abort();
    }
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

void DoProgram(CodeCPU* CPU_code)
{
    ASSERT(CPU_code != NULL);
    size_t cmd = 0;
    stk stk1 ={};
    StackCtor(&stk1, 0);
    CPU_code->stk = &stk1;
    void(**command)(CodeCPU*);
    command = CreateArrayCmd(MAX_CODE_CMD);

    while (CPU_code->ip < CPU_code->nCmd)
    {
        ASSERT(CPU_code->stk != NULL);
        //printf("%llu\n", CPU_code->ip);
        cmd = (size_t)(*CMD_BYIT(CPU_code->bin_buf, CPU_code->ip));
        command[cmd](CPU_code);
        StackDump(CPU_code->stk);
    }
    DtorCPU(CPU_code);
    free(command);
}

void(**CreateArrayCmd(size_t number_cmd))(CodeCPU*) 
{
    void(**CMD)(CodeCPU*);
    CMD = (void(**)(CodeCPU*))calloc(number_cmd, sizeof(void(*)));

    CMD[CMD_PUSH] = Push_CMD;
    CMD[CMD_ADD] = Add_CMD;
    CMD[CMD_OUT] = Out_CMD;
    CMD[CMD_SUB] = Sub_CMD;
    CMD[CMD_MUL] = Mul_CMD;
    CMD[CMD_DIV] = Div_CMD;
    CMD[CMD_IN]  = In_CMD;
    CMD[CMD_POP] = Pop_CMD;
    CMD[CMD_HLT] = Hlt_CMD;
    return CMD;
}

void Push_CMD(CodeCPU* CPU_code)
{
    double val = 0;
    if (*MEM_BYIT(CPU_code->bin_buf, CPU_code->ip))
    {
        val = CPU_code->ram[*(size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip)];
    }
    else if (*REG_BYIT(CPU_code->bin_buf, CPU_code->ip))
        val = (double)CPU_code->reg[*(size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip)];
    
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
        if (*MEM_BYIT(CPU_code->bin_buf, CPU_code->ip))
        {
            CPU_code->ram[*(size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip)] = Pop(CPU_code->stk);
        }
        else if (*REG_BYIT(CPU_code->bin_buf, CPU_code->ip))
        {
             CPU_code->reg[*(size_t*)VAL_BYIT(CPU_code->bin_buf, CPU_code->ip)] = Pop(CPU_code->stk);
        }          
    }
    else 
        Pop(CPU_code->stk);
    CPU_code->ip++;
}

void In_CMD(CodeCPU* CPU_code)
{
    double x = 0;
    printf("enter the number\n");
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
    printf("end of process");
    CPU_code->ip = CPU_code->nCmd;
}
