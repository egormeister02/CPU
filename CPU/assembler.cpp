#include "assm.h"

int main()
{
    TEXT assm_text = {};
    TEXT CPU_text  = {};


    FILE* source = fopen(ASSM_FILE, "r");
    FILE* outfile = fopen(SOFT_CPU_FILE, "w");
    
    CreateText(&assm_text, source);

    CreateSoftCPU(&assm_text, &CPU_text);

    printf("%d %d\n", CPU_text.nlines, CPU_text.size);


    fwrite(CPU_text.buf, CPU_text.size, 1, outfile);

    printf("OK\n");


    fclose(source);
    fclose(outfile);
    free(assm_text.buf);
    free(assm_text.Lines);
    return 0;
}