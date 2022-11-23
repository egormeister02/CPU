#include <stdio.h>
#include <stdlib.h>

#define StackCheck(Stack) \
    StackCheckFunc(Stack, #Stack, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define StackDump(Stack) \
    StackDumpFunc(Stack, #Stack, __FILE__, __LINE__, __PRETTY_FUNCTION__, StackCheck(Stack))


#define POISON POISON_DOUBLE
const char NAME_LOG_FILE[] = "D:\\VScode_projects\\CPU\\Stack\\LogStack.txt";

typedef double Elem_t;

extern FILE* LogStack;

struct stk
{
    Elem_t* data = NULL;
    size_t size = 0;
    size_t capacity = 0;
};

enum ResizeMode 
{
    UP = 0,
    DOWN = 1
};

enum StackCodeError
{
    STACK_OK                 = 0,
    STACK_NULL               = 1,
    STACK_DESTRUCT           = 2,
    STACK_DATA_NULL          = 3,
    STACK_SIZE_LESS_0        = 4,
    STACK_SIZE_MORE_CAPACITY = 5
};

const size_t DESTRUCT_DATA = 0x66DEAD66;
const size_t DESTRUCT_SIZE = 0x77DEAD77;
const size_t DESTRUCT_CAPACITY = 0x77DEAD77;
const size_t BASE_CAPACITY = 10;
const int POISON_INT = 0x7DED;
const char POISON_CHAR = 127;
const long POISON_LONG = 0x7ADEADA7;
const float POISON_FLOAT = 0xFDEADF;
const double POISON_DOUBLE = 0xFFDEADFF;
const int MAX_SIZE_STR = 40;


void StackCtor(stk*, size_t);

void Push(stk*, Elem_t);

Elem_t Pop(stk*);

void StackResize(stk*, ResizeMode);

void StackDtor(stk*);

StackCodeError StackCheckFunc(const stk* st,const char name[MAX_SIZE_STR], const char file[MAX_SIZE_STR], int line, const char func[MAX_SIZE_STR]);

FILE* StartLog(void);

void FinishLog();

void StackDumpFunc(const stk* stk, const char StackName[MAX_SIZE_STR], const char file[MAX_SIZE_STR], size_t line, const char func[MAX_SIZE_STR], const StackCodeError err);

void PrintError(StackCodeError);

void StackPoison(stk*);
