#include "Stack/Stack.h"
#include "config_cmd.h"

#define CMD_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 7)
#define ARG_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 0)
#define MEM_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 1)
#define REG_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 2)
#define VAL_BYIT(buf, index) (char*)((char*)buf + (index * 16) + 8)

#define ASSERT(condition)                                         \
if (!(condition)){                                                \
    fprintf(stdout, "Error in %s:\n"                              \
                    "FILE: %s\n"                                  \
                    "LINE: %d\n"                                  \
                    "FUNCTION: %s\n",                             \
           #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
    abort();}

const char   SOFT_CPU_FILE[]   =   "D:\\VScode_projects\\CPU\\a.code.txt";
const int    CPU_SIGNATURE     =           0xBD;
const int    CPU_VERSION       =              1;
const size_t MAX_CODE_CMD      =              9;
const size_t SIZE_RAM          =            128;       
const size_t SIZE_REG          =             16;

struct CodeCPU
{
    int     signature  =        0;
    int     version    =        0;
    size_t  nCmd       =        0;
    size_t  ip         =        0;
    void*   bin_buf    =     NULL;
    double* ram        =     NULL;
    double  reg[SIZE_REG] =    {};
    stk*    stk        =       {};
};

void ReadHead(CodeCPU*, FILE*);

void CreateCPU(CodeCPU*, FILE*);

void DtorCPU(CodeCPU*);

void DoProgram(CodeCPU*);

void(**CreateArrayCmd(size_t))(CodeCPU*);

void Push_CMD(CodeCPU*);

void Add_CMD(CodeCPU*);

void Sub_CMD(CodeCPU*);

void Mul_CMD(CodeCPU*);

void Div_CMD(CodeCPU*);

void In_CMD(CodeCPU*);

void Pop_CMD(CodeCPU*);

void Out_CMD(CodeCPU*);

void Hlt_CMD(CodeCPU*);
