//#include<conio.h>
#include "asm.h"

int main(int argc, char *argv[]) 
{
    // Проверка наличия аргумента командной строки (имени входного файла)
    if (argc != 2) {
        printf("Use: %s <input_file>\n", argv[0]);
        return 1;
    }

    char* assm_filename = argv[1];

    size_t assm_name_len = strlen(assm_filename);

    char code_filename[assm_name_len + 6];

    MakeCodeFileName(assm_filename, code_filename);

    struct asmtok    assm = {};
    struct CodeCPU CPU_code  = {};

    FILE* source = fopen(assm_filename, "r");
    FILE* codefile = fopen(code_filename, "w+b");
    ASSERT(source != NULL);
    ASSERT(codefile != NULL);

    CreateAsm(&assm, source);

    CreateCPUbuf(&assm, &CPU_code);

    WriteCodeFile(&CPU_code, codefile);

    if (CPU_code.error)
        printf("\nERRORS: %d\n", CPU_code.error);
    
    fclose(source);
    fclose(codefile);

    return 0;
} 

