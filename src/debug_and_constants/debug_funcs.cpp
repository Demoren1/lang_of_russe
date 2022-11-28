#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/debug.h"
#include "../include/stack_objects.h"

static FILE *log_file = 0;

unsigned int stack_error(Stack *stk)                //todo check stk and syk->data departly
{
    int stk_is_null = CHECK(!stk,                 STACK_ERROR_STK_WRONG_PTR);
    
    if(!(stk_is_null & STACK_ERROR_STK_WRONG_PTR) && ((stk->code_of_error & STACK_ERROR_DOUBLE_CTOR) == 0)  && ((stk->code_of_error & STACK_ERROR_DOUBLE_DTOR) == 0))
    {
        stk->code_of_error |= CHECK(!stk->data,   STACK_ERROR_MEMNULL_BUFF); 

        stk->code_of_error |= CHECK(stk->size < 0,                  STACK_ERROR_SIZE_SMALLER_ZERO);
        
        stk->code_of_error |= CHECK(stk->capacity < 0,              STACK_ERROR_CAPACITY_SMALLER_ZERO);
    
        stk->code_of_error |= CHECK(stk->size > stk->capacity,      STACK_ERROR_SIZE_BIGGER_CAPACITY);

        stk->code_of_error |= CHECK(stk->flag & WRONG_REALLOC,      STACK_ERROR_WRONG_REALLOC);

        ON_CANARY_PROT
        (
        stk->code_of_error |= CHECK(stk->l_canary != STRUCT_CANARY, STACK_ERROR_LEFT_CANARY_DIED);
        
        stk->code_of_error |= CHECK(stk->r_canary != STRUCT_CANARY, STACK_ERROR_RIGHT_CANARY_DIED);
        )   
        if(!(stk->code_of_error & STACK_ERROR_MEMNULL_BUFF))
       {
            ON_CANARY_PROT
            (
            stk->code_of_error |= CHECK((size_t) *((size_t*)((char*) stk->data - sizeof(ARR_CANARY))) != ARR_CANARY,                               STACK_ERROR_ARR_LEFT_CANARY_DIED);
            
            stk->code_of_error |= CHECK((size_t) *((size_t*)((char*)stk->data +  stk->capacity * sizeof(elem) /*+ sizeof(ARR_CANARY)*/)) != ARR_CANARY, STACK_ERROR_ARR_RIGHT_CANARY_DIED);
            )

            ON_HASH_PROT
            (
            stk->code_of_error |= CHECK(stk->hash != hash(stk->data, stk->capacity * sizeof(elem)),                  STACK_ERROR_WRONG_HASH);

            stk->code_of_error |= CHECK(stk->hash_struct != hash(stk, sizeof(Stack) - sizeof(stk->hash_struct) - 4), STACK_ERROR_WRONG_STRUCT_HASH);
            )
        }
    }
    else 
    {   
        if (stk_is_null == 0)
        {
            stack_err_decoder(stk->code_of_error);
            return stk->code_of_error;
        }
        stack_err_decoder(stk_is_null);
        abort();
    }

    return stk->code_of_error;
}

void stack_err_decoder(unsigned int code_of_error)
{
    PRINT_ERROR(code_of_error, STACK_ERROR_STK_WRONG_PTR);
 
    PRINT_ERROR(code_of_error, STACK_ERROR_MEMNULL_BUFF);
    
    PRINT_ERROR(code_of_error, STACK_ERROR_SIZE_SMALLER_ZERO);
    
    PRINT_ERROR(code_of_error, STACK_ERROR_CAPACITY_SMALLER_ZERO);
    
    PRINT_ERROR(code_of_error, STACK_ERROR_SIZE_BIGGER_CAPACITY);

    PRINT_ERROR(code_of_error, STACK_ERROR_LEFT_CANARY_DIED);

    PRINT_ERROR(code_of_error, STACK_ERROR_RIGHT_CANARY_DIED);
    
    PRINT_ERROR(code_of_error, STACK_ERROR_ARR_LEFT_CANARY_DIED);
    
    PRINT_ERROR(code_of_error, STACK_ERROR_ARR_RIGHT_CANARY_DIED);

    PRINT_ERROR(code_of_error, STACK_ERROR_WRONG_HASH);

    PRINT_ERROR(code_of_error, STACK_ERROR_WRONG_STRUCT_HASH);

    PRINT_ERROR(code_of_error, STACK_ERROR_DOUBLE_CTOR);

    PRINT_ERROR(code_of_error, STACK_ERROR_DOUBLE_DTOR);

    PRINT_ERROR(code_of_error, STACK_ERROR_CAPACITY_SMALLER_ZERO);

    PRINT_ERROR(code_of_error, STACK_ERROR_DOUBLE_DTOR);

    PRINT_ERROR(code_of_error, STACK_ERROR_POP_FROM_VOID_STACK);

    PRINT_ERROR(code_of_error, STACK_ERROR_WRONG_REALLOC);

    PRINT_ERROR(code_of_error, STACK_ERROR_VALUE_PTR_NULL);

}

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, unsigned int flag_of_error)
{   
    if (!(stk->code_of_error & STACK_ERROR_MEMNULL_BUFF || stk->code_of_error & STACK_ERROR_STK_WRONG_PTR))
    {   

        fprintf(log_file, "%s() at %s (%d):\n", name_of_inner_func, name_of_inner_file, num_of_inner_str);
        fprintf(LOG_FILE, "Stack[%p] (%s) \"%s\" at %s() at %s (%zd) \n", stk, (flag_of_error > 0) ? "ERROR" : "OK",
            stk->dump_info.name_of_variable, stk->dump_info.name_of_func, stk->dump_info.name_of_file, stk->dump_info.num_of_str);

        fprintf(LOG_FILE, "{\n \t size                = %zd;\n"         \
                            "\t capacity            = %zd \n"           \
                            "\t data[%p]                 \n"            \
             ON_CANARY_PROT("\t left_arr_canary     = 0x%0lx\n"          \
                            "\t right_arr_canary    = 0x%0lx\n"          \
                            "\t left_struct_canary  = 0x%0lx\n"          \
                            "\t right_struct_canary = 0x%0lx\n"          )\
               ON_HASH_PROT("\t hash                = 0x%0lx\n"          \
                            "\t struct hash         = 0x%0lx\n"),
                        stk->size, stk->capacity, stk->data ON_CANARY_PROT(COMMA)      //todo in one ON_CANARY_PROT
                        ON_CANARY_PROT((size_t) *((size_t*)((char*) stk->data - sizeof(ARR_CANARY)))) ON_CANARY_PROT(COMMA)
                        ON_CANARY_PROT((size_t) *((size_t*)((char*)stk->data + stk->capacity * sizeof(elem)))) ON_CANARY_PROT(COMMA)
                        ON_CANARY_PROT(stk->l_canary) ON_CANARY_PROT(COMMA)
                        ON_CANARY_PROT(stk->r_canary) ON_HASH_PROT(COMMA)
                        ON_HASH_PROT(stk->hash)       ON_HASH_PROT(COMMA)  
                        ON_HASH_PROT(stk->hash_struct) );
        for (int i = 0; i < stk->capacity; i++)
        {
            if (isnan(stk->data[i])) 
                fprintf(LOG_FILE,"   [%d] = %s\n", i, "NAN(POISON)");
            else 
                fprintf(LOG_FILE," * [%d] = %g\n", i, stk->data[i]);
        }
        END_DUMP();
    }
    else
    {
        fprintf(LOG_FILE, "Dump cant write anything\n");
    } 

}

long hash(void* v_arr, size_t size)
{
    char* arr = (char*) v_arr;

    long hash = 0;
    unsigned int tmp1  = 0, tmp2 = 0;
    for (size_t i = 0; i < size; i++)
    {
        hash += ((int) arr[i]) & 0x0000000F;

        tmp1 = 0xFFF00000;
        tmp2 = 0x00000FFF;

        tmp1 = ((hash & tmp1) >> 20) & tmp2;
        hash <<= 12;
        hash |= tmp1; 

    }
    return hash;
}

int stack_print_in_logs(int line, const char *function, const char *file_name)
{
    fprintf(LOG_FILE, "ERROR: Something go wrong on %d line, %s func, %s file\n", line, function, file_name);

    return 0;
}

void open_logs()
{
    log_file = open_with_no_buff("log_file.txt", "w");
    assert(log_file != NULL);
}

void close_logs()
{
    fclose(LOG_FILE);
}