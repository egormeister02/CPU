#include <conio.h>
#include "Stack.h"

FILE* LogFile = StartLog();

int main()
{
    
    struct stk st;
    StackCtor(&st, 0);
    
    for (int i = 0; i < 76; i++){
    Push(&st, 10);
    Push(&st, 20);
}
    Pop(&st); 
    Pop(&st); 
    
    StackDump(&st);
    StackDtor(&st);
    FinishLog();
    
    //printf("%0x\n", DESTRUCT);
}