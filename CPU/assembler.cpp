#include<conio.h>
#include "assm.h"

int main()
{
    
    struct TEXT    assm_text = {};
    struct CodeCPU CPU_code  = {};


    FILE* source = fopen(ASSM_FILE, "r");
    FILE* codefile = fopen(SOFT_CPU_FILE, "w+b");
    FILE* LogFile = fopen("logText.txt", "w");
    CreateText(&assm_text, source);
    TextDumpFunc(&assm_text, LogFile);

    CreateCPUbuf(&assm_text, &CPU_code);

    //TextDumpFunc(&CPU_text, LogFile);
    fwrite(CPU_code.bin_buf, CPU_code.nCmd * 2, sizeof(size_t), codefile);

    printf("OK\n");
    
    fseek(codefile, 0, SEEK_SET);
    void* buf = (void*)calloc(20, sizeof(size_t));
    fread(buf, sizeof(size_t), 8, codefile);
    for (int i = 0; i < 8; i+=2)
    {
        printf(" %lf\n", *(double*)((double*)buf + (i+1)));
    }
    
    fclose(LogFile);
    fclose(source);
    fclose(codefile);
    free(assm_text.buf);
    free(assm_text.Lines);
    /*
    char*  sval = NULL;
    char* sval1 = NULL;

        sval = (char*)calloc(MAX_LENGTH_VAL, sizeof(char));
        ASSERT(sval != NULL);

    scanf("%s", sval);
    sval1 = IsMem(sval);

    printf("%s", sval1);
*/
    return 0;
}