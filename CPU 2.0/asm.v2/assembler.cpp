//#include<conio.h>
#include "asm.h"

int main()
{
    
    struct asmtok    assm = {};
    struct CodeCPU CPU_code  = {};

    FILE* source = fopen(ASSM_FILE, "r");
    FILE* codefile = fopen(SOFT_CPU_FILE, "w+b");
    ASSERT(source != NULL);
    ASSERT(codefile != NULL);

    CreateAsm(&assm, source);

    CreateCPUbuf(&assm, &CPU_code);

    WriteCodeFile(&CPU_code, codefile);

    printf("\nERRORS: %d\n", CPU_code.error);
    
    fclose(source);
    fclose(codefile);

    return 0;
}