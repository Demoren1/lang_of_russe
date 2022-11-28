#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "../include/asm.h"
#include "../include/stack_objects.h"
#include "../include/debug.h"
#include "../include/cpu.h"

static FILE* cpu_logs = 0;

void open_cpu_logs()
{
    cpu_logs = open_with_no_buff("cpu_logs.txt", "w");
    assert(cpu_logs != NULL);
}

void close_cpu_logs()
{
    fclose(cpu_logs);
}

void dump_cpu(Cpu_struct *cpu, ssize_t ip, Stack *stk, const char* file_name, const char* func_name, int num_of_line)
{
    fprintf(cpu_logs, "\n\nDump was called from %s file, %s function, %d\n", file_name, func_name, num_of_line);
    
    fputs("\nDUMP OF NUM_BUFFER\n", cpu_logs);                               
    for (ssize_t i = 0; i < cpu->num_of_commands; i++)               
    {   if (i < ip)                                               
            fprintf(cpu_logs, "         * [%3zd] = %2d\n", i, cpu->num_buffer[i]);

        else if (i > ip)                                               
            fprintf(cpu_logs, "           [%3zd] = %2d\n", i, cpu->num_buffer[i]); 

        else if (i == ip)
            fprintf(cpu_logs, "  ip = %zd >>[%3zd] = %2d\n", ip,  i, cpu->num_buffer[i]); 
    }                                                                      

    fputs("\nDUMP OF REGISTERS\n", cpu_logs);
    for (int i = 1; i < 5; i++)
    {
        fprintf(cpu_logs, "r%cx = %d\n", 'a' + i - 1, cpu->registers[i]);
    }

    // fputs("\nDUMP OF BUFFER\n", cpu_logs);                                    
    // for (ssize_t i = 0; i < cpu->size; i++)                            
    // {                                                                        
    //     fprintf(cpu_logs, "* [%3zd] = %2c (decimal %d)\n", i, cpu->buffer[i], cpu->buffer[i]);       
    // }

    fputs("\nDUMP OF STACK\n", cpu_logs);
    for (int i = 0; i < stk->capacity; i++)
    {
        if (isnan(stk->data[i])) 
            fprintf(cpu_logs,"   [%d] = %s\n", i, "NAN(POISON)");
        else 
            fprintf(cpu_logs," * [%d] = %g\n", i, stk->data[i]);
    }
    
    // printf("capacity %d\n", cpu->func_stack.capacity);
    
    fputs("\nDUMP OF FUNC STACK\n", cpu_logs);
    for (int i = 0; i < cpu->func_stack.capacity; i++)
    {
        if (isnan(cpu->func_stack.data[i])) 
            fprintf(cpu_logs,"   [%d] = %s\n", i, "NAN(POISON)");
        else 
            fprintf(cpu_logs," * [%d] = %g\n", i, cpu->func_stack.data[i]);
    }

    fputs("\nDUMP OF RAM\n", cpu_logs);
    for (int i = 0; i < SIZE_OF_RAM; i++)
    {
        if (cpu->RAM[i] != 0)
            fprintf(cpu_logs, "[%3d] = %3d\n", i, cpu->RAM[i]);
    }
}