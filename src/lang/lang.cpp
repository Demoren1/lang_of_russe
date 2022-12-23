#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <lekser_debug.h>

int main(int argc, char *argv[])         //todo lekser and parser rules
{                                        //todo add error checkers
    open_tree_logs();                    

    Buffer buff = {};
    Tokens data_tokens = {};
    Node *tree = 0;
    
    
    char src[MAX_LEN_VALUE] = {};
    lang_get_name(src, argc, argv);
    char path_asm[MAX_LEN_VALUE] = "codes.asm";
    // const char *path_asm = "codes.asm";

    diff_handle_src(src, &buff);

    lekser_data_tokens_ctor(&data_tokens);

    lekser_handle(&data_tokens, &buff);
    

    tree = get_General(&data_tokens);

    lekser_dump(&data_tokens);
    TREE_DUMP(tree, INORDER);

    lang_trans(tree, path_asm);

    // asm_func(argc, argv);
    // cpu();

    lekser_dtor(&data_tokens);
    diff_buff_dtor(&buff);
    close_tree_logs();
    node_dtor(tree);
    // node_dtor(cool_node);
    return 0;
}

