#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <asm.h>
#include <stack_objects.h>
#include <debug.h>
#include <cpu.h>

int check_executable_file(FILE *exec_file_ptr, Cpu_struct *cpu)
{
    char extension[128] = {};

    int version = 0;

    char first_line[128] = {};

    fgets(first_line, 32, exec_file_ptr);
    
    int shift = 0;
    int delta_shift = 0;

    sscanf(first_line, "%s%n", extension, &shift);
    sscanf(first_line + shift, "%d%n", &version, &delta_shift);
    shift += delta_shift;
    sscanf(first_line + shift, "%zd", &cpu->num_of_commands);

    CHECK_CONDITION(strcmp(extension,cpu->extension) == 0);
    CHECK_CONDITION(version == cpu->version);

    return 0;
}

size_t know_size_for_buff(FILE* text, const char * name_of_file)
{
    CHECK_CONDITION(text != 0);
    CHECK_CONDITION(name_of_file != 0);

    struct stat data = {};
    stat(name_of_file, &data);

    if (((data.st_mode & S_IFMT) - S_IFDIR) == 0)
        {
            printf("Trying to find size of dir");
            abort();
        }

    return data.st_size;
}

int fill_cpu_struct(FILE *codes_file_ptr, Cpu_struct *cpu, const char *path_to_file)
{
    cpu->size = know_size_for_buff(codes_file_ptr, path_to_file);

    cpu->buffer = (char*) calloc(cpu->size, sizeof(char));
    
    int test_fread = 0;
    test_fread = fread(cpu->buffer, sizeof(char), cpu->size, codes_file_ptr);
    CHECK_CONDITION(test_fread != 0);

    make_num_buffer(cpu);

    return 0;
}

void dtor_exec_no_bin(Cpu_struct *cpu)
{
    free(cpu->buffer);
    free(cpu->num_buffer);
}

void make_num_buffer(Cpu_struct *cpu)
{   
    cpu->num_buffer = (int*) calloc(cpu->num_of_commands, sizeof(int));
    
    int shift = 0;
    int delta_shift = 0;
    int num = 0;

    for (ssize_t counter = 0; counter < cpu->num_of_commands; counter++)
    {
        sscanf(cpu->buffer + shift,"%d%n", &num, &delta_shift);
        shift += delta_shift;

        cpu->num_buffer[counter] = num;
    }
}

int do_not_bin_instructions(FILE* exec_not_bin_file_ptr, const char* path_to_executable_file, FILE* file_result)
{
    CHECK_CONDITION(exec_not_bin_file_ptr != 0);
    CHECK_CONDITION(path_to_executable_file != 0);
    CHECK_CONDITION(file_result != 0);
       
    struct Cpu_struct cpu ={};

    open_logs();
    STACK_CTOR(cpu.stack, 10);
    
    STACK_CTOR(cpu.func_stack, 5);

    check_executable_file(exec_not_bin_file_ptr, &cpu);

    fill_cpu_struct(exec_not_bin_file_ptr, &cpu, path_to_executable_file);

    execute_commands(&cpu, &cpu.stack, file_result);

    dtor_exec_no_bin(&cpu);
    
    stack_dtor(&cpu.stack);
    stack_dtor(&cpu.func_stack);
    close_logs();

    return 0;
}

void execute_commands(Cpu_struct *cpu, Stack *stack, FILE* file_result)
{
    ssize_t ip = 0;
    int cmd = 0;
    int full_cmd = 0;

    DUMP_CPU(*cpu, ip, stack); 

#define DEF_CMD(name, num, arg, end_sym, getarg_code,...)                          \
        case num:                                                                  \
            __VA_ARGS__                                                            \
            break;                                                                 \

    while (ip < cpu->num_of_commands) // ToDo: use file size
    {   elem num1 = 0, num2 = 0, num = 0;
        cmd = cpu->num_buffer[ip++];
        full_cmd = cmd;
        (cmd != DUMP_CMD) && (cmd = cmd & MASK_CMD);

        // printf("cmd = %0x (%d)\n", cmd, cmd);
        // printf("full_cmd = %0x(%d) \n", full_cmd, full_cmd);
        
        if (cmd == HLT_CMD)
        {
            break;            
        }
        switch (cmd)
        {
            #include "../include/cmd.h"
        }
        DUMP_CPU(*cpu, ip, &cpu->stack);
    }
}
#undef DEF_CMD


int do_bin_instructions(FILE* exec_bin_file_ptr, const char* path_to_executable_file_bin, FILE* file_result)
{   
    CHECK_CONDITION(exec_bin_file_ptr != 0);
    CHECK_CONDITION(path_to_executable_file_bin != 0);
    CHECK_CONDITION(file_result != 0);

    struct Cpu_struct cpu ={};  

    Stack stack ={};
    open_logs();

    STACK_CTOR(cpu.stack, 10);
    
    STACK_CTOR(cpu.func_stack, 5);
    
    check_executable_bin_file(exec_bin_file_ptr, &cpu);

    fill_cpu_struct_bin(exec_bin_file_ptr, &cpu, path_to_executable_file_bin);
    
    execute_commands(&cpu, &cpu.stack, file_result);
    
    dtor_exec_no_bin(&cpu);
    
    stack_dtor(&cpu.stack);
    stack_dtor(&cpu.func_stack);
    close_logs();

    return 0;
}

int check_executable_bin_file(FILE *exec_file_bin, Cpu_struct *cpu)
{
    int first_line[FIRST_LINE_LEN] = {};   
    
    fread(first_line, sizeof(int), 2, exec_file_bin);
    
    CHECK_CONDITION(first_line[0] == EXTENSION[0] + EXTENSION[1] * 256); 
    CHECK_CONDITION(first_line[1] == VERSION);

    return 0;
}

int fill_cpu_struct_bin (FILE *exec_file_bin, Cpu_struct *cpu, const char *path_to_bin_file)
{   
    CHECK_CONDITION(exec_file_bin != 0);
    CHECK_CONDITION(cpu != 0);
    CHECK_CONDITION(path_to_bin_file != 0);

    find_num_of_commands(exec_file_bin, cpu, path_to_bin_file);
    
    cpu->size = 0;

    cpu->num_buffer = (int*) calloc(cpu->num_of_commands, sizeof(int));

    fread(cpu->num_buffer, sizeof(int), cpu->num_of_commands, exec_file_bin);

    return 0;
}

int find_num_of_commands(FILE *exec_file_bin, Cpu_struct *cpu, const char *name_of_file)
{
    int max_commands = know_size_for_buff(exec_file_bin, name_of_file);
    max_commands /= sizeof(int);

    int first_line[FIRST_LINE_LEN] = {};
    CHECK_CONDITION(first_line != 0);
    
    int test_fread = 0;
    test_fread = fread(first_line, sizeof(int), 1, exec_file_bin);
    CHECK_CONDITION(test_fread != 0);

    cpu->num_of_commands = (first_line[0] < max_commands) ? max_commands : first_line[0];
    
    return 0;
}

void dtor_exec_bin(Cpu_struct *cpu)
{   
    free(cpu->buffer);
    free(cpu->num_buffer);
}

