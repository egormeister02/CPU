#include "Onegin.h"
#include "config_cmd.h"
#include <math.h>


const char   ASSM_FILE[]       =     "assm.txt";
const char   SOFT_CPU_FILE[]   =   "a.code.txt";
const int    MAX_SIZE_COMMAND  =             10;
const int    MAX_LENGTH_VAL    =             10;
const int    MAX_CPU_STR       =             22;
const int    NAN_VAL           =              0;
const double POISON_VAL        =     0xFDEFFEDF;
void CreateCPUtext(const TEXT*, TEXT*);

void CreateCPUcode(const TEXT*, TEXT*);

size_t SizeVal(int);

void TextDumpFunc(const TEXT*, FILE*);