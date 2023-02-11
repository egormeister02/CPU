#include "Stack.h"

FILE* LogStack = StartLog();

void StackCtor(stk* stk, size_t capacity)
{
    if (stk == NULL) StackCheck(stk);

    stk->size = 0;
    stk->capacity = (capacity == 0 ? BASE_CAPACITY: capacity);
    stk->buf = (void*)calloc(stk->capacity * sizeof(Elem_t) + 2 * SIZE_CANNARY, sizeof(char));
    stk->data = (Elem_t*)((char*)stk->buf + SIZE_CANNARY);

#if CANNARY_GUARD
    LEFT_CANNARY(stk)   = CANNARY;
    RIGHT_CANNARY(stk)  = CANNARY;
    stk->left_cannary   = CANNARY;
    stk->right_cannary  = CANNARY;
#endif

    StackPoison(stk);

#if HASH_GUARD
    stk->hash = HashFunc(stk);
#endif

    StackCheck(stk);
}


void Push(stk* stk, Elem_t val)
{
    if (stk->err) return;
    StackCheck(stk);

    StackResize(stk, UP);

    stk->data[stk->size++] = val;

#if HASH_GUARD
    stk->hash = HashFunc(stk);
#endif

    StackCheck(stk);
}

Elem_t Pop(stk* stk)
{
    if (stk->err) return 0;
    StackCheck(stk)0;

    Elem_t value = stk->data[--stk->size];
    stk->data[stk->size] = POISON;

#if HASH_GUARD
    stk->hash = HashFunc(stk);
#endif

    StackResize(stk, DOWN);

    StackCheck(stk)0;
    return value;
}

void StackDtor(stk* stk)
{

    stk->data = (Elem_t*)DESTRUCT_DATA;
    stk->size = DESTRUCT_SIZE;
    stk->capacity = DESTRUCT_CAPACITY;
    free(stk->buf);
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

    void* temp_buf = (void*)realloc((char*)stk->buf, NewCapacity * sizeof(Elem_t) + SIZE_CANNARY * 2);

    if (temp_buf == NULL)
    {
        stk->err = OUT_OF_MEMORY;
        StackDumpFunc(stk, "stk", __FILE__, __LINE__, __PRETTY_FUNCTION__, stk->err);
        return;
    }

    stk->buf = temp_buf;
    stk->data = (Elem_t*)((char*)stk->buf + SIZE_CANNARY);
    stk->capacity = NewCapacity;

#if CANNARY_GUARD
    RIGHT_CANNARY(stk) = CANNARY;
#endif
    
    StackPoison(stk);

#if HASH_GUARD
    stk->hash = HashFunc(stk);
#endif

    StackCheck(stk);
}

StackCodeError StackCheckFunc(const stk* st,const char name[MAX_SIZE_STR], 
                              const char file[MAX_SIZE_STR], int line, const char func[MAX_SIZE_STR])
{
    if (st == NULL)
    {        
        PrintInfo(" is NULL:");      
        StackDumpFunc(st, name, file, line, func, STACK_NULL);
        return STACK_NULL;
    }
    
    else if ((st->data == (Elem_t*)DESTRUCT_DATA) || (st->size == DESTRUCT_SIZE) || (st->capacity == DESTRUCT_CAPACITY))
    {            
        PrintInfo(" is destructed:");               
        StackDumpFunc(st, name, file, line, func, STACK_DESTRUCT);
        return STACK_DESTRUCT;
    }

    else if (st->buf == NULL)
    {                                           
        PrintInfo("->buf == NULL:");
        StackDumpFunc(st, name, file, line, func, STACK_DATA_NULL);
        return STACK_DATA_NULL;
    }
#if CANNARY_GUARD
    else if (st->left_cannary != CANNARY)
    {    
        PrintInfo(": dead left struct cannary:");  
        StackDumpFunc(st, name, file, line, func, STACK_STRUCT_LEFT_CANN);
        return STACK_STRUCT_LEFT_CANN;
    }
    else if (st->right_cannary != CANNARY)
    {                                           
        PrintInfo(": dead right struct cannary:"); 
        StackDumpFunc(st, name, file, line, func, STACK_STRUCT_RIGHT_CANN);
        return STACK_STRUCT_RIGHT_CANN;
    }
    else if (LEFT_CANNARY(st) != CANNARY)
    {                                           
        PrintInfo(": dead left cannary:");  
        StackDumpFunc(st, name, file, line, func, STACK_LEFT_CANN);
        return STACK_LEFT_CANN;
    }
    else if (RIGHT_CANNARY(st) != CANNARY)
    {                                           
        PrintInfo(": dead right cannary:"); 
        StackDumpFunc(st, name, file, line, func, STACK_RIGHT_CANN);
        return STACK_RIGHT_CANN;
    }
#endif
#if HASH_GUARD
    else if (st->hash != HashFunc(st))
    {                                    
        PrintInfo(": invalid hash");        
        StackDumpFunc(st, name, file, line, func, STACK_WRONG_HASH);
        return STACK_WRONG_HASH;
    }
#endif
    else if ((long)st->size < 0)
    {                                      
        PrintInfo("->size < 0:");      
        StackDumpFunc(st, name, file, line, func, STACK_SIZE_LESS_0);
        return STACK_SIZE_LESS_0;
    }
    else if ((long)st->size >= (long)st->capacity)
    {                                      
        PrintInfo("->size >= st.capacity:");      
        StackDumpFunc(st, name, file, line, func, STACK_SIZE_MORE_CAPACITY);
        return STACK_SIZE_MORE_CAPACITY;
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
    fprintf(LogStack, "\n------------------------------Finish Logging-------------------------------\n");
    fclose (LogStack);
    LogStack = NULL;
}

void StackDumpFunc(const stk* stk, const char StackName[MAX_SIZE_STR], const char file[MAX_SIZE_STR],
                   size_t line, const char func[MAX_SIZE_STR], const StackCodeError err)
{   
    fprintf(LogStack, "\n----------------------------------StackDump----------------------------------\n");
   
    if (err == STACK_NULL)
    {
        PrintError(err);
        fprintf(LogStack, "pointer of %s is NULL\n", StackName);
        fprintf(LogStack, "Called at %s at %s(%llu)\n", file, func, line);
        return;
    }

    fprintf(LogStack, "Called at %s at %s(%llu)\n", file, func, line);
    
    PrintError(err);

#if CANNARY_GUARD
    fprintf(LogStack, "\n    CANNARY = %llX\n", CANNARY);
#endif

#if HASH_GUARD
    fprintf(LogStack, "\n    HashFunc value = %lld\n\n", HashFunc(stk));
#endif
    
    fprintf(LogStack, "{\n\n    ---------------------------------------\n");

#if CANNARY_GUARD
    fprintf(LogStack, "    left struct cannary  = %llX\n\n", stk->left_cannary);
#endif

    fprintf(LogStack, "    data pointer         = %p\n", stk->data);    
    fprintf(LogStack, "    size                 = %llu\n", stk->size);
    fprintf(LogStack, "    capacity             = %llu\n", stk->capacity); 

#if HASH_GUARD
    fprintf(LogStack, "    hash                 = %llu\n", stk->hash); 
#endif

#if CANNARY_GUARD
    fprintf(LogStack, "\n    right struct cannary = %llX\n", stk->right_cannary);
#endif

    fprintf(LogStack, "    ---------------------------------------\n\n");

    if ((err == STACK_DATA_NULL) || (err == STACK_DESTRUCT))     
        return;
    else
    {
        fprintf(LogStack, "    {\n");

#if CANNARY_GUARD
    fprintf(LogStack, "    LEFT_CANNARY = %llX\n\n", LEFT_CANNARY(stk));
#endif

    
        char* comment;
        for (size_t index = 0; index < stk->capacity; index++)
        {
            fprintf(LogStack, "\t");
            fprintf(LogStack, (index < stk->size) ? "*" : " ");
            fprintf(LogStack, "[%llu] = ", index);
            comment = (char*)(stk->data[index] == POISON ? "(POISON)": "");
            fprintf(LogStack, "%lf %s", stk->data[index], comment);

            fprintf(LogStack, "\n");
        }

#if CANNARY_GUARD
    fprintf(LogStack, "\n    RIGHT_CANNARY = %llX\n", RIGHT_CANNARY(stk));
#endif        
        fprintf(LogStack, "    }\n");
    }

    fprintf(LogStack, "}\n");
    fprintf(LogStack, "\n---------------------------------------------------------------------------\n");
}

void PrintError(const StackCodeError err)
{
    switch (err)
    {
    case STACK_NULL:
        fprintf(LogStack, "!!!__STACK POINTER == NULL__!!!\n ");
        break;
    case STACK_OK:
        fprintf(LogStack, "Stack is OK\n");
        break;
    case STACK_DESTRUCT:
        fprintf(LogStack, "!!!__STACK IS DESTRUCTED__!!!\n");
        break;
    case STACK_DATA_NULL:
        fprintf(LogStack, "!!!__STACK_BUF == NULL__!!!\n ");
        break;
    case STACK_SIZE_LESS_0:
        fprintf(LogStack, "!!!__STACK_SIZE < 0__!!!\n");
        break;
    case STACK_SIZE_MORE_CAPACITY:
        fprintf(LogStack, "!!!__STACK_SIZE > STACK_CAPACITY__!!!\n");
        break;
    case OUT_OF_MEMORY:
        fprintf(LogStack, "!!!__OUT_OF_MEMORY__!!!\n");
        break;
    case STACK_LEFT_CANN:
        fprintf(LogStack, "!!!__LEFT CANNARY DEAD__!!!\n");
        break;
    case STACK_RIGHT_CANN:
        fprintf(LogStack, "!!!__RIGHT CANNARY DEAD__!!!\n");
        break;
    case STACK_STRUCT_LEFT_CANN:
        fprintf(LogStack, "!!!__LEFT STRUCT CANNARY DEAD__!!!\n");
        break;
    case STACK_STRUCT_RIGHT_CANN:
        fprintf(LogStack, "!!!__RIGHT STRUCT CANNARY DEAD__!!!\n");
        break;
    case STACK_WRONG_HASH:
        fprintf(LogStack, "!!!__HASH DOESN'T MATCH WITH HASH_FUNC VALUE__!!!\n");
        break;
    
    default: break;
    }
}

void StackPoison(stk* stk)
{
    for (size_t i = stk->size; i < stk->capacity; i++)
        stk->data[i] = POISON;
}

#if HASH_GUARD
    size_t HashFunc(const stk* stk)
    {
        size_t s = 0;
        for (size_t i = 0; i < stk->capacity; i++)
        {
            s += (((long long)stk->data[i] / ((i+1)*5 % 43 + 1)) + (((long long)stk->data[i]) % ((i+1)*5 % 23 + 1))*((i+1) % 29));
            s = s % 4147483647;
        }
        return s;
    }
#endif