#ifndef DEBUG_HEADER_GUARD
#define DEBUG_HEADER_GUARD

#include <unistd.h>

void open_logs();
void close_logs();

#define DBG printf("%s:%d -- %s\n", __FILE__, __LINE__, __FUNCTION__);

#define COMMA ,

#define END_DUMP() fputs("-----------------------------------------------------------------------------------------------------------------------\n\n", log_file);

#define LOG_FILE (assert(log_file != NULL && "Maybe you forgot to open log_file"), log_file)                               

#define SHOW_ELEMENTS(stk, file)    for(ssize_t counter = stk.size - 1; counter >= 0; counter--)          \
                                    {                                                                     \
                                        fprintf(file, "[%3ld] = %3g, ", counter, stk.data[counter]);  \
                                        if (counter % 10 == 0)                                            \
                                            fprintf(file, "\n");                                      \
                                    }                                                                     \
                                    fprintf(file, "\n");                 

#define WRITE_LOG (...) 

#define DUMP_ASS(structure, size)   dump_ass(structure, size, FUNC_GENERAL_INFO())

#define FUNC_GENERAL_INFO()  __FILE__, __FUNCTION__, __LINE__

#define DUMP_CPU(info_of_executable_file, ip, stk)    dump_cpu(&info_of_executable_file, ip, stk, FUNC_GENERAL_INFO());      

#define ASSERTED() stack_print_in_logs(__LINE__, __FUNCTION__, __FILE__);

#define CHECK_FUNC(func)    if(func == -1)                              \
                            {                                           \
                                printf("Something go wrong in func ");  \
                                DBG;                                    \
                                return -1;                              \
                            }

#define CHECK_CONDITION(condition)  if(!condition)                           \
                                    {                                        \
                                        printf("Something go wrong on");     \
                                        DBG;                                 \
                                        return -1;                           \
                                    }  
                 
enum errors 
{
    STACK_ERROR_MEMNULL_BUFF          = 1 << 0,
    STACK_ERROR_SIZE_SMALLER_ZERO     = 1 << 1,
    STACK_ERROR_CAPACITY_SMALLER_ZERO = 1 << 2,
    STACK_ERROR_SIZE_BIGGER_CAPACITY  = 1 << 3,
    STACK_ERROR_RIGHT_CANARY_DIED     = 1 << 4,
    STACK_ERROR_LEFT_CANARY_DIED      = 1 << 5,
    STACK_ERROR_ARR_RIGHT_CANARY_DIED = 1 << 6,
    STACK_ERROR_ARR_LEFT_CANARY_DIED  = 1 << 7,
    STACK_ERROR_WRONG_HASH            = 1 << 8,
    STACK_ERROR_WRONG_STRUCT_HASH     = 1 << 9,
    STACK_ERROR_DOUBLE_CTOR           = 1 << 10,
    STACK_ERROR_DOUBLE_DTOR           = 1 << 11,
    STACK_ERROR_STK_WRONG_PTR         = 1 << 12,
    STACK_ERROR_POP_FROM_VOID_STACK   = 1 << 13,
    STACK_ERROR_WRONG_REALLOC         = 1 << 14,
    STACK_ERROR_VALUE_PTR_NULL        = 1 << 15 
};

enum errors_in_process
{
    STACK_CREATED              = 1 << 0,
    STACK_DESTROYED            = 1 << 1,
    WRONG_REALLOC              = 1 << 2,
    VALUE_PTR_NULL             = 1 << 3
};

#define CHECK(condition, code_of_error)  (condition) ? code_of_error : 0;

#define PRINT_ERROR(testing_var, code_of_error) (testing_var & code_of_error) ? fprintf(LOG_FILE, "%s\n", #code_of_error) : 0;

#define HASH_PROT

#ifdef HASH_PROT
#define ON_HASH_PROT(part_of_hash_prot) part_of_hash_prot 
#else 
#define ON_HASH_PROT(part_of_hash_prot1)
#endif

#define CANARY_PROT

#ifdef CANARY_PROT
#define ON_CANARY_PROT(part_of_canary_prot) part_of_canary_prot
#else 
#define ON_CANARY_PROT(part_of_canary_prot)
#endif

#endif 