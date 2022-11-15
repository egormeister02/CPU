#include "Onegin.h"
#include "config_cmd.h"
#include <math.h>

struct CodeCPU
{
    int     signature  =    0;
    int     version    =    0;
    size_t  nElem      =    0;
    void*   bin_buf    = NULL;
};

const char   ASSM_FILE[]       =     "assm.txt";
const char   SOFT_CPU_FILE[]   =   "a.code.txt";
const int    CPU_SIGNATURE     =           0xBD;
const int    CPU_VERSION       =              1;
const int    MAX_SIZE_COMMAND  =             10;
const int    MAX_LENGTH_VAL    =             10;
const int    MAX_CPU_STR       =             22;
const int    NAN_VAL           =              0;
const double POISON_VAL        =     0xFDEFFEDF;

void CreateCPUtext(const TEXT*, TEXT*);

void CodeCPUCtor(const TEXT*, CodeCPU*);

void CreateCPUbuf(const TEXT*, CodeCPU*);

void CreateCPUcode(const TEXT*, TEXT*);

size_t SizeVal(int);

void TextDumpFunc(const TEXT*, FILE*);

void CreateCodeFile(const TEXT*, FILE*);