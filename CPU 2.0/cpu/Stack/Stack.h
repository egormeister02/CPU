#include <stdio.h>
#include <stdlib.h>

#define CANNARY_GUARD 1
#define HASH_GUARD    1

#if CANNARY_GUARD

    #define LEFT_CANNARY(stk) *((size_t*)stk->buf)
    #define RIGHT_CANNARY(stk) *((size_t*)((char*)stk->buf + SIZE_CANNARY + stk->capacity * sizeof(Elem_t)))

    const size_t SIZE_CANNARY = sizeof(size_t);
#else
    const size_t SIZE_CANNARY =              0;
#endif

#define StackCheck(Stack)                                                                                   \
    if ((Stack->err = StackCheckFunc(Stack, #Stack, __FILE__, __LINE__, __PRETTY_FUNCTION__)) != STACK_OK)   \
        return

#define StackDump(Stack) \
    StackDumpFunc(Stack, #Stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, StackCheckFunc(Stack, #Stack, __FILE__, __LINE__, __PRETTY_FUNCTION__))

#define PrintInfo(error)                                            \
    fprintf(stdout, "Error in: %s%s\n"                              \
                        "FILE: %s\n"                                \
                        "LINE: %d\n"                                \
                        "FUNCTION: %s\n"                            \
                        "StackDump!!!\n",                           \
                name, error, file, line, func);                            

extern FILE* LogStack;

const char NAME_LOG_FILE[] = "LogStack.txt";
const size_t CANNARY           = 0xFCECCACF;

#define POISON POISON_DOUBLE
typedef double Elem_t;

enum ResizeMode 
{
    UP = 0,
    DOWN = 1
};

enum StackCodeError
{
    STACK_OK                 =  0,
    STACK_NULL               =  1,
    STACK_DESTRUCT           =  2,
    STACK_DATA_NULL          =  3,
    STACK_SIZE_LESS_0        =  4,
    STACK_SIZE_MORE_CAPACITY =  5,
    OUT_OF_MEMORY            =  6,
    STACK_LEFT_CANN          =  7,
    STACK_RIGHT_CANN         =  8,
    STACK_STRUCT_LEFT_CANN   =  9,
    STACK_STRUCT_RIGHT_CANN  = 10,
    STACK_WRONG_HASH         = 11
};

struct stk
{
#if CANNARY_GUARD
    size_t left_cannary = 0;
#endif

    Elem_t* data       =      NULL;
    void*   buf        =      NULL;
    size_t  size       =         0;
    size_t  capacity   =         0;
    StackCodeError err =  STACK_OK;

#if HASH_GUARD
    size_t hash = 0;
#endif

#if CANNARY_GUARD
    size_t right_cannary = 0;
#endif
};



const size_t DESTRUCT_DATA     = 0x66DEAD66;
const size_t DESTRUCT_SIZE     = 0x77DEAD77;
const size_t DESTRUCT_CAPACITY = 0x77DEAD77;
const size_t BASE_CAPACITY     =         10;
const int    POISON_INT        =     0x7DED;
const char   POISON_CHAR       =        127;
const long   POISON_LONG       = 0x7ADEADA7;
const float  POISON_FLOAT      =   0xFDEADF;
const double POISON_DOUBLE     = 0xFFDEADFF;
const size_t HASH_MOD          = 2147483647;
const int    MAX_SIZE_STR      =         40;


void StackCtor(stk*, size_t);

void Push(stk*, Elem_t);

Elem_t Pop(stk*);

void StackResize(stk*, ResizeMode);

void StackDtor(stk*);

StackCodeError StackCheckFunc(const stk* st,const char name[MAX_SIZE_STR], const char file[MAX_SIZE_STR],
                                 int line, const char func[MAX_SIZE_STR]);

FILE* StartLog(void);

void FinishLog();

void StackDumpFunc(const stk* stk, const char StackName[MAX_SIZE_STR], const char file[MAX_SIZE_STR],
                   size_t line, const char func[MAX_SIZE_STR], const StackCodeError err);

void PrintError(const StackCodeError);

void StackPoison(stk*);

#if HASH_GUARD
    size_t HashFunc(const stk*);
#endif