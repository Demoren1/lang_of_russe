#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/debug.h"
#include "../include/stack_objects.h"

int stack_ctor(Stack *stk, ssize_t capacity, const char* name_function, const char* name_file, const char* name_variable, int num_line)            
{   
    assert(stk != NULL);                        //todo resize to create

    if(stk->flag & STACK_CREATED)
    {
        stk->code_of_error |= STACK_ERROR_DOUBLE_CTOR;

        ASSERT_OK(stk);
    }
    assert(stk->data == nullptr);
    stk->data   = (elem*) calloc(1, capacity * (sizeof(elem)) ON_CANARY_PROT(+ 2 * sizeof(canary_t))); 
    
    ON_CANARY_PROT 
    (
    (*((canary_t*)stk->data) = ARR_CANARY);   
    *((canary_t*)((char*)stk->data +  capacity * sizeof(elem) + sizeof(canary_t))) = ARR_CANARY; //todo  write(1, ptr, 0) == -1
    stk->data = (elem*)((canary_t*)stk->data + 1);
    )

    stk->capacity      = capacity;
    stk->size          = 0;
    stk->code_of_error = 0;
    stk->flag         |= STACK_CREATED;

    ON_CANARY_PROT
    (
    stk->r_canary = STRUCT_CANARY;
    stk->l_canary = STRUCT_CANARY;
    )

    ON_HASH_PROT(stack_rehash(stk));       
    
    ASSERT_OK(stk);
    
    stk->dump_info ={};
    stack_dump_info_ctor(stk, name_function, name_file, name_variable, num_line);

    ON_HASH_PROT(stack_rehash(stk));

    stack_poison_get(stk, stk->size, stk->capacity);
    
    ASSERT_OK(stk);
    
    return stk->code_of_error;
}   

int stack_dump_info_ctor(Stack *stk, const char* name_function, const char* name_file, const char* name_variable, int num_line)
{   
    ASSERT_OK(stk);

    stk->dump_info.name_of_func     = name_function;
    stk->dump_info.name_of_file     = name_file;   
    stk->dump_info.name_of_variable = name_variable + 1;
    stk->dump_info.num_of_str       = num_line;

    ON_HASH_PROT(stack_rehash(stk));

    ASSERT_OK(stk);
    return stk->code_of_error;
}

int stack_push(Stack *stk, elem value)
{   
    ASSERT_OK(stk);

    if (stk->size + 1 >= stk->capacity)
    {            
       stack_resize(stk, stk->capacity * 2);
    }
    
    ON_HASH_PROT(stack_rehash(stk));
    ASSERT_OK(stk);

    stk->data[stk->size] = value;
    stk->size++;

    ON_HASH_PROT(stack_rehash(stk));

    ASSERT_OK(stk);
        
    return stk->code_of_error;
}

elem stack_pop(Stack *stk, elem *value)
{   
    
    ASSERT_OK(stk);
    
    if (stk->size <= 0)
    {
        stk->code_of_error |= STACK_ERROR_POP_FROM_VOID_STACK;
        ASSERT_OK(stk);
    }

    if(value == NULL)
    {
        stk->code_of_error |= STACK_ERROR_VALUE_PTR_NULL;
        ASSERT_OK(stk);
    }

    if ((stk->size+1) * 4 < stk->capacity)
    {
        stack_resize(stk, stk->capacity / 2);
    }
    
    *value = stk->data[stk->size-1];
    
    stack_poison_get(stk, stk->size, stk->capacity);
    stk->size--;

    ON_HASH_PROT(stack_rehash(stk));
    ASSERT_OK(stk);
    
    return stk->code_of_error;
}

int stack_dtor(Stack *stk)
{   
    if(stk->flag & STACK_DESTROYED)
    {
        stk->code_of_error |= STACK_ERROR_DOUBLE_DTOR;
        stack_err_decoder(stk->code_of_error);
        return stk->code_of_error;
    }
    
    stk->flag |= STACK_DESTROYED;
    ON_HASH_PROT(stack_rehash(stk));
    
    
    ASSERT_OK(stk);
    
    stack_poison_get(stk, 0, stk->capacity);
    stk->capacity = -1;
    stk->size     = -1;
    
    free((char*) stk->data ON_CANARY_PROT(- sizeof(canary_t)));
    //todo destroy data ptr
    
    return 0;
}

int stack_resize(Stack *stk, ssize_t new_capacity)
{   
    ASSERT_OK(stk);
    
    stk->data = (elem*) realloc((char*)stk->data ON_CANARY_PROT(- sizeof(canary_t)), new_capacity * (sizeof(elem)+1) ON_CANARY_PROT(+ 2 * sizeof(elem)));
    
    if (stk->data == NULL)
    {   
        stk->capacity = -1;
        stk->flag |= WRONG_REALLOC;

        ASSERT_OK(stk);
    }

    stk->capacity = new_capacity;
    ON_CANARY_PROT
    (
    *((canary_t*)stk->data) = ARR_CANARY;                   //todo to func
    *((canary_t*)((char*)stk->data +  new_capacity * sizeof(elem) + sizeof(canary_t))) = ARR_CANARY;
    stk->data   = (elem*)((canary_t*)stk->data + 1);
    )

    ON_HASH_PROT(stack_rehash(stk));
    
    stack_poison_get(stk, stk->size, stk->capacity);
    
    ASSERT_OK(stk);    

    return stk->code_of_error;
}

int stack_poison_get(Stack *stk, size_t size, size_t capacity)
{
    ASSERT_OK(stk);
    size_t i = size;
    for (; i < capacity; i++)
    {
        *(stk->data + i) = NAN;
    }

    ON_HASH_PROT(stack_rehash(stk));
    ASSERT_OK(stk);
    return stk->code_of_error;
}

ON_HASH_PROT
(
void stack_rehash(Stack *stk)
{   
    stk->hash        = hash(stk->data, stk->capacity * sizeof(elem));
    stk->hash_struct = hash(stk, sizeof(Stack) - sizeof(stk->hash_struct) - 4);
}
)