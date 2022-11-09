#include "Stack.h"

void StackCtor(stk* stk, size_t capacity)
{
    if (stk == NULL) StackCheck(stk);

    stk->size = 0;
    stk->capacity = (capacity == 0 ? BASE_CAPACITY: capacity);
    stk->data = (Elem_t*)calloc(stk->capacity, sizeof(Elem_t));
    StackPoison(stk);

    StackCheck(stk);
}


void Push(stk* stk, int x)
{
    StackCheck(stk);

    StackResize(stk, UP);

    stk->data[stk->size++] = x;

    StackCheck(stk);
}

Elem_t Pop(stk* stk)
{
    StackCheck(stk);

    Elem_t value = stk->data[--stk->size];
    stk->data[stk->size] = POISON;

    StackResize(stk, DOWN);

    StackCheck(stk);
    return value;
}

void StackDtor(stk* stk)
{
    StackCheck(stk);

    stk->data = (Elem_t*)DESTRUCT_DATA;
    stk->size = DESTRUCT_SIZE;
    stk->capacity = DESTRUCT_CAPACITY;
}

void StackResize(stk* stk, ResizeMode mode)
{
    StackCheck(stk);

    size_t NewCapacity = 0;

    if (mode == UP)
    {
        if ((long)stk->size < (long)(stk->capacity-1))
            return;
        else 
            NewCapacity = (stk->capacity == 0 ? BASE_CAPACITY: stk->capacity*2);
    }

    if (mode == DOWN)
    {
        if (((long)stk->size < (long)(stk->capacity / 4)) & ((long)stk->size > (long)BASE_CAPACITY))
            NewCapacity = stk->capacity / 2;
        else 
            return;
    }

    stk->data = (Elem_t*)realloc((char*)stk->data, NewCapacity * sizeof(Elem_t));

    stk->capacity = NewCapacity;
    StackPoison(stk);

    StackCheck(stk);
}

StackCodeError StackCheckFunc(const stk* st,const char name[MAX_SIZE_STR], const char file[MAX_SIZE_STR], int line, const char func[MAX_SIZE_STR])
{
    if (st == NULL)
    {                                           
    fprintf(stdout, "Error in: %s is NULL:\n"                          
                    "FILE: %s\n"                               
                    "LINE: %d\n"                                
                    "FUNCTION: %s\n"
                    "StackDump!!!\n",                            
            name, file, line, func);  
    StackDumpFunc(st, name, file, line, func, STACK_NULL);
    FinishLog();
    abort();
    }
    else if ((st->data == (Elem_t*)DESTRUCT_DATA) || (st->size == DESTRUCT_SIZE) || (st->capacity == DESTRUCT_CAPACITY))
    {                                           
    fprintf(stdout, "Error in: %s is destructed:\n"                          
                    "FILE: %s\n"                               
                    "LINE: %d\n"                                
                    "FUNCTION: %s\n"
                    "StackDump!!!\n",                            
            name, file, line, func);  
    StackDumpFunc(st, name, file, line, func, STACK_DESTRUCT);
    FinishLog();
    abort();
    }
    else if (st->data == NULL)
    {                                           
    fprintf(stdout, "Error in %s->data == NULL:\n"                          
                    "FILE: %s\n"                               
                    "LINE: %d\n"                                
                    "FUNCTION: %s\n"
                    "StackDump!!!\n",                            
            name, file, line, func);  
    StackDumpFunc(st, name, file, line, func, STACK_DATA_NULL);
    FinishLog();
    abort();
    }
    else if ((long)st->size < 0)
    {                                           
    fprintf(stdout, "Error in %s->size < 0:\n"                          
                    "FILE: %s\n"                               
                    "LINE: %d\n"                                
                    "FUNCTION: %s\n"
                    "StackDump!!!\n",                            
            name, file, line, func);  
    StackDumpFunc(st, name, file, line, func, STACK_SIZE_LESS_0);
    FinishLog();
    abort();
    }
    else if ((long)st->size >= (long)st->capacity)
    {                                           
    fprintf(stdout, "Error in %s->size >= st.capacity:\n"                          
                    "FILE: %s\n"                               
                    "LINE: %d\n"                                
                    "FUNCTION: %s\n"
                    "StackDump!!!\n",                            
            name, file, line, func);  
    StackDumpFunc(st, name, file, line, func, STACK_SIZE_MORE_CAPACITY);
    FinishLog();
    abort();
    }
    return STACK_OK;
}


FILE* StartLog(void)
{
    FILE* logfile = fopen(NAME_LOG_FILE, "w");
    fprintf(logfile, "--------------------------------Start Logging--------------------------------\n");
    return logfile;
}

void FinishLog(void)
{
    fprintf(LogFile, "\n------------------------------Finish Logging-------------------------------\n");
    fclose (LogFile);
    LogFile = NULL;
}

void StackDumpFunc(const stk* stk, const char StackName[MAX_SIZE_STR], const char file[MAX_SIZE_STR], size_t line, const char func[MAX_SIZE_STR], const StackCodeError err)
{   
    fprintf(LogFile, "\n----------------------------------StackDump----------------------------------\n");
   
    if (err == STACK_NULL)
    {
        fprintf(LogFile, "pointer of %s is NULL\n", StackName);
        fprintf(LogFile, "Called at %s at %s(%u)\n", file, func, line);
        return;
    }

    fprintf(LogFile, "Called at %s at %s(%u)\n", file, func, line);
    
    PrintError(err);
    
    fprintf(LogFile, "{\n");

    fprintf(LogFile, "    data pointer = %p\n", stk->data);
    
    if ((err == STACK_DATA_NULL) || (err == STACK_DESTRUCT))
    {        
        fprintf(LogFile, "    size         = %lx\n", stk->size);
        fprintf(LogFile, "    capacity     = %lx\n", stk->capacity);

    }
    else
    {
        fprintf(LogFile, "    size         = %lu\n", stk->size);
        fprintf(LogFile, "    capacity     = %lu\n", stk->capacity);

        fprintf(LogFile, "    {\n");
        char* comment;
        for (size_t index = 0; index < stk->capacity; index++)
        {
            fprintf(LogFile, "\t");
            fprintf(LogFile, (index < stk->size) ? "*" : " ");
            fprintf(LogFile, "[%lu] = ", index);
            comment = (char*)(stk->data[index] == POISON ? "(POISON)": "");
            fprintf(LogFile, "%d %s", stk->data[index], comment);

            fprintf(LogFile, "\n");
        }
        fprintf(LogFile, "    }\n");
    }

    fprintf(LogFile, "}\n");
    fprintf(LogFile, "\n---------------------------------------------------------------------------\n");

}

void PrintError(StackCodeError err)
{
    switch (err)
    {
    case STACK_OK:
        fprintf(LogFile, "Stack is OK\n");
        break;
    case STACK_DESTRUCT:
        fprintf(LogFile, "!!!__STACK IS DESTRUCTED__!!!\n");
        break;
    case STACK_DATA_NULL:
        fprintf(LogFile, "!!!__STACK_DATA == NULL__!!!\n ");
        break;
    case STACK_SIZE_LESS_0:
        fprintf(LogFile, "!!!__STACK_SIZE < 0__!!!\n");
        break;
    case STACK_SIZE_MORE_CAPACITY:
        fprintf(LogFile, "!!!__STACK_SIZE > STACK_CAPACITY__!!!\n");
        break;
    }
}

void StackPoison(stk* stk)
{
    StackCheck(stk);
    for (int i = (long)stk->size; i < (long)stk->capacity; i++)
        stk->data[i] = POISON;
}