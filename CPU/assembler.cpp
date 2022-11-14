#include<conio.h>
#include "assm.h"

int main()
{
    TEXT assm_text = {};
    TEXT CPU_text  = {};


    FILE* source = fopen(ASSM_FILE, "r");
    FILE* outfile = fopen(SOFT_CPU_FILE, "w");
    FILE* LogFile = fopen("logt.txt", "w");
    CreateText(&assm_text, source);
    TextDumpFunc(&assm_text, LogFile);
    printf("OK\n");
    //printf("%d\n",assm_text.Lines[0].length);
    //printf("%u\n", assm_text.Lines[0].length);

    CreateCPUcode(&assm_text, &CPU_text);

    //printf("%llu %llu\n", CPU_text.nlines, CPU_text.size);

    TextDumpFunc(&CPU_text, LogFile);
    fwrite(CPU_text.buf, CPU_text.size, 1, outfile);

    printf("OK\n");

    fclose(LogFile);
    fclose(source);
    fclose(outfile);
    free(assm_text.buf);
    free(assm_text.Lines);
    return 0;
}