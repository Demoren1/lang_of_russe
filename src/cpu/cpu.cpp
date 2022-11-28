#include <stdio.h>
#include <assert.h>
#include<string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <asm.h>
#include <stack_objects.h>
#include <debug.h>
#include <cpu.h>

int main()
{   
    open_cpu_logs();

    const char *path_to_executable_file = "executable_file.txt";
    const char *path_to_executable_file_bin = "executable_file.bin";
    const char *path_result_of_executable = "result_of_executable_file.txt";
    
    FILE* stk_out_file   = fopen("elements_of_stack.txt", "w");
    // FILE* exec_not_bin_file_ptr = fopen(path_to_executable_file, "r");
    FILE* exec_bin_file_ptr = fopen(path_to_executable_file_bin, "rb");
    FILE* file_result = fopen(path_result_of_executable, "w");
    FILE* file_result_bin = fopen(path_result_of_executable, "w");

    // CHECK_CONDITION(exec_not_bin_file_ptr != 0);
    CHECK_CONDITION(stk_out_file != 0);
    CHECK_CONDITION(file_result != 0);

    //do_not_bin_instructions(exec_not_bin_file_ptr, path_to_executable_file, file_result);
    
    do_bin_instructions(exec_bin_file_ptr, path_to_executable_file_bin, file_result);
    
    // fclose(exec_not_bin_file_ptr);
    fclose(file_result);
    fclose(stk_out_file);
    fclose(exec_bin_file_ptr);
    fclose(file_result_bin);
    close_cpu_logs();
    
    return 1;
}
