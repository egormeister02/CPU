#include "CPU.h"

int main()
{
    ASSERT(LogStack != NULL);
    
    FILE* codefile = fopen(SOFT_CPU_FILE, "r+b");

    struct CodeCPU CPU_code = {};

    CreateCPU(&CPU_code, codefile);

    DoProgram(&CPU_code);
    
    
    //FinishLog();
}