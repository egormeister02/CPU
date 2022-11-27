#include "CPU.h"

int main()
{
    FILE* codefile = fopen(SOFT_CPU_FILE, "r+b");
    struct CodeCPU CPU_code = {};

    CreateCPU(&CPU_code, codefile);
    //stk stk1 = {};
    //StackCtor(&stk1, 0);
    //CPU_code.stk = &stk1;
    DoProgram(&CPU_code);
    
    FinishLog();
}