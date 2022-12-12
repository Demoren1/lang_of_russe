#include <stdio.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <calcul_funcs.h>


Node *diff()
{
    // open_tree_logs();

    Tree tree = {};
    Buffer buff = {};

    const char *src = "expression.txt";
    diff_handle_src(src, &buff);

    char *str = buff.buffer;
    
    printf("str = %s\n", buff.buffer);

    tree.root = get_General(str);
    
    Node *cool_node = node_copy_node(tree.root);
    diff_simplify(cool_node);
    TREE_DUMP(cool_node, INORDER);

    #if 0
    Var var_arr[MAX_VARS] = {{"xss", 1}};
    int n_vars = diff_ctor_var_arr(cool_node, var_arr, 0);
    // int n_vars = 1;
    double result = 0;
    result = diff_calc_tree(cool_node, var_arr, n_vars); 
    char tmp_tailor_str[MAX_LEN_VALUE] = "xss";
    double tailor_row_result = diff_tailor_one_var(cool_node, 10, tmp_tailor_str, 1, 0);

    printf("vars = %d, result = %g\n",n_vars, result);
    printf("Tailor row result = %g\n", tailor_row_result);
    #endif

    #if 1
    char var_str[MAX_LEN_VALUE] = "x";
    Node *diff_cool_node = diff_diff(cool_node, var_str);
    diff_simplify(diff_cool_node);
    TREE_DUMP(diff_cool_node, INORDER)
    node_dtor(diff_cool_node);
    #endif

    node_dtor_calloc_data(tree.root, buff.buffer, buff.size);
    node_dtor(tree.root);
    diff_buff_dtor(&buff);

    // close_tree_logs();

    return cool_node;
}