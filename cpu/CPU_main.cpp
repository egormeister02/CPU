#include "CPU.h"

int main(int argc, char *argv[]) 
{
    // Проверка наличия аргумента командной строки (имени входного файла)
    if (argc != 2) {
        printf("Use: %s <input_file>\n", argv[0]);
        return 1;
    }

    char* code_filename = argv[1];

    if (FileNameVerify(code_filename, ".code")) return 1;

    FILE* codefile = fopen(code_filename, "r+b");

    struct CodeCPU CPU_code = {};

    CreateCPU(&CPU_code, codefile);

    DoProgram(&CPU_code);
    
    FinishLog();
}