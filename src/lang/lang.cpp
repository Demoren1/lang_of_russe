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
#include <string.h>

int main(int argc, char *argv[])
{
    open_tree_logs();

    Buffer buff = {};
    Tokens data_tokens = {};
    
    const char *src = "expression.txt";
    const char *path_asm = "codes.txt";
    diff_handle_src(src, &buff);

    // char str[256] = "lall lololo hohohoh";
    // char *ptr = strtok(str, " ");

    // printf("stroka: %s\n", ptr);

    lekser_data_tokens_ctor(&data_tokens);

    lekser_handle(&data_tokens, &buff);

    for (int i = 0; i < data_tokens.size; i++)
    {
        Token *cur_token = data_tokens.tokens[i];
        printf("type: %d ", cur_token->type_node);

        switch(cur_token->type_node)
        {
            case NUM:
                printf("value: %g \n", cur_token->value.dbl_value);
                break;
            case ARITHM_OP:
                printf("value: %d \n", cur_token->value.op_value);
                break;
            case LOG:
                printf("value: %d \n", cur_token->value.log_op);
                break;
            case VAR:
                printf("value: %s \n", cur_token->value.var_value);
                break;
            default:
                printf("error");
        }
    }

    // Node **nodes = (Node **) calloc(buff.lines, sizeof(Node));
    
    #if 0
    printf("lines = %ld\n", buff.lines);

    for (int index = 0; index < buff.lines; index++)
    {
        printf("str: %s\n", buff.buffer);
        
        nodes[index] = get_General(buff.buffer);
        diff_simplify(nodes[index]);

        TREE_DUMP(nodes[index], INORDER);
        buff.buffer += lang_to_new_line(buff.buffer);
    }

    lang_write_asm(nodes, buff.lines, path_asm);

    
    asm_func(argc, argv);
    cpu();
    #endif
    
    // lang_dtor_nodes(nodes, buff.lines);
    // free(nodes);

    lekser_dtor(&data_tokens);
    diff_buff_dtor(&buff);
    close_tree_logs();
    return 0;
}

