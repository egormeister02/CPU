#include "Onegin/Onegin.h"
#include "config_cmd.h"

#define CMD_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 7)
#define ARG_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 0)
#define MEM_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 1)
#define REG_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 2)
#define VAL_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 8)

struct CodeCPU
{
    int     signature  =    0;
    int     version    =    0;
    size_t  nCmd      =    0;
    void*   bin_buf    = NULL;
};

const char   ASSM_FILE[]       =      "D:\\VScode_projects\\CPU\\asm.txt";
const char   SOFT_CPU_FILE[]   =   "D:\\VScode_projects\\CPU\\a.code.txt";
const int    CPU_SIGNATURE     =           0xBD;
const int    CPU_VERSION       =              1;
const int    MAX_SIZE_COMMAND  =             10;
const int    MAX_LENGTH_VAL    =             10;
const int    MAX_CPU_STR       =             22;
const int    NAN_VAL           =              0;
const long   POISON_VAL        =     0xFDEFFEDF;

void CreateCPUtext(const TEXT*, TEXT*);

void CodeCPUCtor(const TEXT*, CodeCPU*);

void CreateCPUbuf(const TEXT*, CodeCPU*);

void CreateCPUcode(const TEXT*, TEXT*);

size_t SizeVal(int);

void WriteCodeFile(const CodeCPU*, FILE*);

char* IsMem(char*);

size_t IsReg(const char*);

