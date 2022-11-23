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
    CPU_code->nCmd = (size_t)*((size_t*)((size_t*)head + 1));
    free(head);
}

void CreateCPU(CodeCPU* CPU_code, FILE* codefile)
{
    ASSERT(CPU_code != NULL);
    ASSERT(codefile != NULL);

    ReadHead(CPU_code, codefile);

    CPU_code->bin_buf = (void*)calloc(CPU_code->nCmd * 2, sizeof(size_t));
    ASSERT(CPU_code->bin_buf != NULL);

    fread(CPU_code->bin_buf, sizeof(size_t), CPU_code->nCmd * 2, codefile);
}

void DoProgram(CodeCPU* CPU_code)
{
    ASSERT(CPU_code != NULL);
    struct stk stk1 = {};
    size_t ip = 0;
    size_t cmd = 0;

    StackCtor(&stk1, 0);

    void(**command)(stk*, void*);
    command = CreateArrayCmd(MAX_CODE_CMD);

    while (ip < CPU_code->nCmd)
    {
        cmd = (size_t)(*CMD_BYIT(CPU_code->bin_buf, ip));
        command[cmd](&stk1, ARG_BYIT(CPU_code->bin_buf, ip));
        //StackDump(&stk1);
        ip++;
    }
    StackDtor(&stk1);
    free(command);
}

void(**CreateArrayCmd(size_t number_cmd))(stk*, void*) 
{
    void(**CMD)(stk*, void*);
    CMD = (void(**)(stk*, void*))calloc(number_cmd, sizeof(void(*)));

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

void Push_CMD(stk* stk, void* buf)
{
    Push(stk, *(double*)VAL_BYIT(buf, 0));
}

void Add_CMD(stk* stk, void* buf)
{
    Push(stk, Pop(stk) + Pop(stk));
}

void Sub_CMD(stk* stk, void* buf)
{
    Push(stk, Pop(stk) - Pop(stk));
}

void Mul_CMD(stk* stk, void* buf)
{
    Push(stk, Pop(stk) * Pop(stk));
}

void Div_CMD(stk* stk, void* buf)
{
    Push(stk, Pop(stk) / Pop(stk));
}

void Pop_CMD(stk* stk, void* buf)
{
    Pop(stk);
}

void In_CMD(stk* stk, void* buf)
{
    double x = 0;
    printf("enter the number\n");
    scanf("%lf", &x);
    Push(stk, x);
}

void Out_CMD(stk* stk, void* buf)
{
    printf("%lf\n", Pop(stk));
}

void Hlt_CMD(stk* stk, void* buf)
{
    printf("end of process");
}
