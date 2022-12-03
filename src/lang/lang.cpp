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

    Buffer buff = {};
    
    const char *src = "expression.txt";
    diff_handle_src(src, &buff);
    
    Node **nodes = (Node **) calloc(buff.lines, sizeof(Node));

    printf("lines = %ld\n", buff.lines);

    for (int index = 0; index < buff.lines; index++)
    {
        printf("str: %s\n", buff.buffer);
        nodes[index] = get_General(buff.buffer);
        diff_simplify(nodes[index]);
        TREE_DUMP(nodes[index], INORDER);
        buff.buffer += lang_to_new_line(buff.buffer);
    }

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

    lang_dtor_nodes(nodes, buff.lines);
    free(nodes);
    close_tree_logs();
    return 0;
}

