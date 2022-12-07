#include<conio.h>
#include "asm.h"

FILE* ListFile;

int main()
{
    
    struct asmtok    assm = {};
    struct CodeCPU CPU_code  = {};

    ListFile = fopen(LIST_FILE, "w");
    FILE* source = fopen(ASSM_FILE, "r");
    FILE* codefile = fopen(SOFT_CPU_FILE, "w+b");
    ASSERT(source != NULL);
    ASSERT(codefile != NULL);

    fprintf(ListFile, "\n-------------------------------StartListing-------------------------------\n\n");
    fprintf(ListFile, "  NumTok  |  Line  |   str     |  code/val   | MEM | REG |   pos  |  comment  \n");

    CreateAsm(&assm, source);

    CreateCPUbuf(&assm, &CPU_code);

    WriteCodeFile(&CPU_code, codefile);

    printf("ERRORS: %d\n", CPU_code.error);
    
    fclose(source);
    fclose(codefile);
    //DtorText(&assm_text);
    return 0;
}