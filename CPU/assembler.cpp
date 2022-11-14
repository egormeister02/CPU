#include<conio.h>
#include "assm.h"

int main()
{
    TEXT assm_text = {};
    TEXT CPU_text  = {};


    FILE* source = fopen(ASSM_FILE, "r");
    FILE* codefile = fopen(SOFT_CPU_FILE, "w");
    FILE* LogFile = fopen("logText.txt", "w");
    CreateText(&assm_text, source);
    TextDumpFunc(&assm_text, LogFile);

    CreateCPUcode(&assm_text, &CPU_text);

    TextDumpFunc(&CPU_text, LogFile);
    fwrite(CPU_text.buf, CPU_text.size, 1, codefile);

    printf("OK\n");

    fclose(LogFile);
    fclose(source);
    fclose(codefile);
    free(assm_text.buf);
    free(assm_text.Lines);
    return 0;
}