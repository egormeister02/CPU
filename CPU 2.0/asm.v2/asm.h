
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
#define JMP_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 3)
#define VAL_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 8)

const unsigned char MAX_TYPE_TOK = 10; 
const size_t        SIZE_JARG    = 20;

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
    JMP  = CMD_JMP,

    NUM  = MAX_TYPE_TOK + 1,
    MEM  = MAX_TYPE_TOK + 2,
    REG  = MAX_TYPE_TOK + 3,
    JMA  = MAX_TYPE_TOK + 4,
    JML  = MAX_TYPE_TOK + 5
};

enum typejump
{
    JUN  = CMD_JMP,
    JB   =  CMD_JB,
    JA   =  CMD_JA,
    JBE  = CMD_JBE,
    JAE  = CMD_JAE,
    JEE  = CMD_JEE,
    JNE  = CMD_JNE
};

enum err
{
    ok,
    mem,
    tok,
    push_arg,
    pop_arg,
    jmp_arg,
    arg_cmd,
    jmp_link
};

struct jmptor
{
    typejump type;
    char val[20];
};

struct Token
{
    char*    str     = NULL;
    size_t   line    =    0;   
    size_t   dval    =    0;
    double   val     =    0;
    typetok  type;
    typejump jtype;
    err      error   =   ok;
};

struct CodeCPU
{
    int     signature  =    0;
    int     version    =    0;
    size_t  nCmd       =    0;
    void*   bin_buf    = NULL;
    int     error      =    0;
};

struct asmtok
{
    char*   buf        = NULL;
    size_t  size       =    0;
    Token*  Toks       = NULL; 
    size_t  nJmp       =    0;
    size_t  nJLin      =    0;
    size_t  nTok       =    0;
    int     error      =    0;
};

struct jump
{
    size_t ip   = 0;
    char*  arg  = NULL;
    size_t line = 0;
};

extern FILE* ListFile;

const char   ASSM_FILE[]       =      "D:\\VScode_projects\\CPU 2.0\\asm.txt";
const char   SOFT_CPU_FILE[]   =   "D:\\VScode_projects\\CPU 2.0\\a.code.bin";
const char   LIST_FILE[]       =  "D:\\VScode_projects\\CPU 2.0\\listing.txt";
const int    CPU_SIGNATURE     =           0xBD;
const int    CPU_VERSION       =              1;
const size_t MAX_JMP_ARG       =            256;
const size_t SIZE_RAM          =            128;       
const size_t SIZE_REG          =             16;

const size_t POISON_VAL        =     0xFDEFFEDF;

void CreateAsm(asmtok*, FILE*);

void CreateToks(asmtok*);

void CreateTypeTok(Token*);

void CodeCPUCtor(const asmtok*, CodeCPU*);

void CreateCPUbuf(const asmtok*, CodeCPU*);

void WriteCodeFile(const CodeCPU*, FILE*);

void PrintErr(err);

int IsMem(const char*);

size_t IsJmpArg(const char*);

size_t IsJmpLin(const char*);

size_t ScanMem(const char*);

double ScanVal(const char*);

size_t IsReg(const char*);

