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

struct CodeCPU
{
    int     signature  =    0;
    int     version    =    0;
    size_t  nCmd       =    0;
    void*   bin_buf    = NULL;
};

const char   SOFT_CPU_FILE[]   =   "D:\\VScode_projects\\CPU\\a.code.txt";
const int    CPU_SIGNATURE     =           0xBD;
const int    CPU_VERSION       =              1;
const size_t MAX_CODE_CMD      =              9;

void ReadHead(CodeCPU*, FILE*);

void CreateCPU(CodeCPU*, FILE*);

void DoProgram(CodeCPU*);

void(**CreateArrayCmd(size_t))(stk*, void*);

void Push_CMD(stk*, void*);

void Add_CMD(stk*, void*);

void Sub_CMD(stk*, void*);

void Mul_CMD(stk*, void*);

void Div_CMD(stk*, void*);

void In_CMD(stk*, void*);

void Pop_CMD(stk*, void*);

void Out_CMD(stk*, void*);

void Hlt_CMD(stk*, void*);
