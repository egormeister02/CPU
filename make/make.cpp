#include <stdlib.h> 
#include <stdio.h>
#include "filename.h"

int main(int argc, char *argv[]) 
{
    // Проверка наличия аргумента командной строки (имени входного файла)
    if (argc != 2) {
        printf("Use: %s <input_file>\n", argv[0]);
        return 1;
    }

    char* assm_filename = argv[1];

    size_t assm_name_len = strlen(assm_filename);

    if (FileNameVerify(assm_filename, ".txt")) return 1;

    char code_filename[assm_name_len + 6];

    MakeCodeFileName(assm_filename, code_filename);

    char command_asm[100] = "./asm ";
    char command_cpu[100] = "./cpu ";

    strcpy(command_asm + 6, assm_filename);
    strcpy(command_cpu + 6, code_filename);

    system(command_asm);
    system(command_cpu);
}
