#include<conio.h>
#include "asm.h"

int main()
{
    
    struct TEXT    assm_text = {};
    struct CodeCPU CPU_code  = {};

    FILE* source = fopen(ASSM_FILE, "r");
    FILE* codefile = fopen(SOFT_CPU_FILE, "w+b");
    ASSERT(source != NULL);
    ASSERT(codefile != NULL);

    CreateText(&assm_text, source);

    CreateCPUbuf(&assm_text, &CPU_code);

    WriteCodeFile(&CPU_code, codefile);

    printf("OK\n");
    
    fclose(source);
    fclose(codefile);
    DtorText(&assm_text);
    return 0;
}