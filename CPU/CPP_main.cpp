#include "Stack.h"

FILE* LogFile = StartLog();

int main()
{
    
    struct stk st;
    StackCtor(&st, 0);
    char comand[10] = "";
    long value = 0;
    do 
    {
        scanf("%s %d", comand, value);
        
    } 
    while (comand != "end");
    
    StackDump(&st);
    StackDtor(&st);
    FinishLog();
    
    //printf("%0x\n", DESTRUCT);
}