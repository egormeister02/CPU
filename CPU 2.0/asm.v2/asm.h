//#include "Onegin/Onegin.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "config_cmd.h"

#define ASSERT(condition)                                         \
if (!(condition)){                                                \
    fprintf(stdout, "Error in %s:\n"                              \
                    "FILE: %s\n"                                  \
                    "LINE: %d\n"                                  \
                    "FUNCTION: %s\n",                             \
           #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
    abort();}

#define CMD_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 7)
#define ARG_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 0)
#define MEM_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 1)
#define REG_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 2)
#define VAL_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 8)

const unsigned char MAX_CODE_CMD = 9; 

enum typetok
{
    PUSH = CMD_PUSH,
    ADD  = CMD_ADD,
    SUB  = CMD_SUB,
    MUL  = CMD_MUL,
    DIV  = CMD_DIV,
    IN   = CMD_IN,
    POP  = CMD_POP,
    OUT  = CMD_OUT,
    HLT  = CMD_HLT,
    NUM  = MAX_CODE_CMD + 1,
    MEM  = MAX_CODE_CMD + 2,
    REG  = MAX_CODE_CMD + 3
};

struct Token
{
    char* str = NULL;
    size_t line = 0;
    typetok type;
    size_t dval = 0;
    double val = 0;
};

struct CodeCPU
{
    int     signature  =    0;
    int     version    =    0;
    size_t  nCmd       =    0;
    void*   bin_buf    = NULL;
};

struct asmtok
{
    char*   buf        = NULL;
    size_t  size = 0;
    Token*  Toks       = NULL;
    size_t  nTok       =    0;
};



const char   ASSM_FILE[]       =      "D:\\VScode_projects\\CPU 2.0\\asm.txt";
const char   SOFT_CPU_FILE[]   =   "D:\\VScode_projects\\CPU 2.0\\a.code.bin";
const int    CPU_SIGNATURE     =           0xBD;
const int    CPU_VERSION       =              1;
const int    MAX_SIZE_COMMAND  =             10;
const int    MAX_LENGTH_VAL    =             10;
const int    MAX_CPU_STR       =             22;

const int    NAN_VAL           =              0;
const size_t POISON_VAL        =     0xFDEFFEDF;

void CreateAsm(asmtok*, FILE*);

void CreateToks(asmtok*);

void CreateTypeTok(Token*);

void CodeCPUCtor(const asmtok*, CodeCPU*);

void CreateCPUbuf(const asmtok*, CodeCPU*);

size_t SizeVal(int);

void WriteCodeFile(const CodeCPU*, FILE*);

int IsMem(const char*);

size_t ScanMem(const char*);

double ScanVal(const char*);

size_t IsReg(const char*);

