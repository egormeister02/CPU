#include<conio.h>
#include "assm.h"

int main()
{
    
    struct TEXT    assm_text = {};
    struct CodeCPU CPU_code  = {};

    FILE* source = fopen(ASSM_FILE, "r");
    FILE* codefile = fopen(SOFT_CPU_FILE, "w+b");

    CreateText(&assm_text, source);

    CreateCPUbuf(&assm_text, &CPU_code);

    fwrite(CPU_code.bin_buf, CPU_code.nCmd * 2, sizeof(size_t), codefile);

    printf("OK\n");
    
    fclose(source);
    fclose(codefile);
    DtorText(&assm_text);
    return 0;
}