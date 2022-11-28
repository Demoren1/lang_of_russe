#ifndef STACK_HEADER_GUARD
#define STACK_HEADER_GUARD

#define ASSERT_OK(stk)      if (stack_error(stk) != 0)                                                      \
                            {                                                                               \
                                stack_err_decoder(stk->code_of_error);                                      \
                                stack_dump(stk, __FUNCTION__, __FILE__, __LINE__, stack_error(stk));        \
                                return 0;                                                                   \
                            }                                                                               \
    

#define VAR_INFO(stack) __FUNCTION__, __FILE__, #stack, __LINE__ //todo stack.dump_info{__FUNCTION__, __FILE__, #stack, __LINE__} 

#define STACK_CTOR(stack, capacity) stack_ctor(&stack, capacity, VAR_INFO(stack))

const size_t STRUCT_CANARY     = 0xF00DB00B;  
const size_t ARR_CANARY        = 0xABCDABEE;   
typedef size_t canary_t;

typedef double elem;

typedef struct                 
{
    const char* name_of_func;
    const char* name_of_file;
    const char* name_of_variable;
    size_t   num_of_str;
}info_of_funcs;

typedef struct  
{
    size_t l_canary;
    elem *data;
    ssize_t size;
    ssize_t capacity;
    info_of_funcs dump_info;
    long hash;
    unsigned int code_of_error = 0;
    char flag = 0;
    size_t r_canary;
    long hash_struct;
}Stack;

unsigned int stack_error(Stack *stk);

int stack_ctor(Stack *stk, ssize_t capacity, const char* name_function, const char* name_file, const char* nmae_variable, int num_line);

int stack_dump_info_ctor(Stack *stk, const char* name_function, const char* name_file, const char* name_variable, int num_line);

void stack_err_decoder(unsigned int code_of_error);

int stack_push(Stack *stk, elem value);

int stack_resize(Stack *stk, ssize_t new_capacity);

elem stack_pop(Stack *stk, elem *value);

int stack_poison_get(Stack *stk, size_t size, size_t capacity);

int stack_dtor(Stack *stk);

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, unsigned int flag_of_error);

long hash(void* arr, size_t size);

void stack_rehash(Stack *stk);

int stack_print_in_logs(int line, const char *function, const char *file_name);

FILE* open_with_no_buff(const char* name_file, const char* regime);

#endif