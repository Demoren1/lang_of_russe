// #include <asm.h>
#include <calcul_funcs.h>
// #include <cmd.h>
// #include <cpu.h>
// #include <debug.h>
// #include <diff_debug.h>
#include <diff_funcs.h>
// #include <dsl.h>
// #include <general_debug.h>
// #include <stack_objects.h>
// #include <tree_debug.h>
// #include <tree_funcs.h>
#include <lang.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    #if 1
    

    Buffer buff = {};

    const char *src = "program.txt";

    diff_handle_src(src, &buff);

    printf("%s\n", buff.buffer);


    asm_func(argc, argv);
    cpu();
    
    #endif 

    return 0;
}

