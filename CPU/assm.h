#include "Onegin.h"
#include "config_cmd.h"
#include <math.h>


const char ASSM_FILE[]     =     "assm.txt";
const char SOFT_CPU_FILE[] = "soft_CPU.txt";
const int MAX_SIZE_COMMAND =             10;
const int  MAX_CPU_STR     =             20;

void CreateCPUtext(const TEXT*, TEXT*);

void CreateSoftCPU(const TEXT*, TEXT*);

int SizeVal(int);