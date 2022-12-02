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
#include <tree_debug.h>
// #include <tree_funcs.h>
#include <lang.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    open_tree_logs();

    Node *root = diff();

    #if 0 
    Buffer buff = {};
    const char *src = "program.txt";

    diff_handle_src(src, &buff);

    printf("%s\n", buff.buffer);
    char *str = buff.buffer;
    Node *root = get_General(str);
    TREE_DUMP(root, INORDER);
    
    // asm_func(argc, argv);
    // cpu();

    node_dtor(root);
    #endif

    node_dtor(root);
    close_tree_logs();
    return 0;
}

