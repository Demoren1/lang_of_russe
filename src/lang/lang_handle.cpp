#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <general_debug.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <lang.h>

static int _write_node_(Node *node, int cur_var_arr_index, FILE *ASM_file);

static int  Print_num(double value, FILE *ASM_file);

static int  Print_var(char var[], int cur_var_arr_index, FILE *ASM_file, int push_flag);

static int  Print_op(Arith_Operation arith_op, FILE *ASM_file);

static int  Print_log_op(Log_Oper log_op, FILE *ASM_file);

static int _print_to_file_(Node *node, int cur_var_arr_index, FILE *ASM_file);

static int _find_var_index_(char var[], int cur_var_arr_index);

Var vars[SIZE_OF_VARS_ARR] = {};

int lang_to_new_line(char buffer[])
{
    int num_sym = 0;

    while(buffer[num_sym] != '\0')
    {
        num_sym++;
    }
    num_sym++;

    return num_sym;
}


int lang_dtor_nodes(Node *nodes[], size_t lines)
{
    for (int i = 0; i < lines; i++)
    {
        node_dtor(nodes[i]);
    }

    return 0;
}

int lang_write_asm(Node *nodes[], size_t lines, const char *path_asm)
{
    FILE *ASM_file = fopen(path_asm, "w");
    int cur_var_arr_index = 0;

    for (int index = 0; index < lines; index++)
    {
        cur_var_arr_index += lang_ctor_var_arr(nodes[index], vars, cur_var_arr_index);
        _write_node_(nodes[index], cur_var_arr_index, ASM_file);
    }

    fclose(ASM_file);
    return 0;
}

int lang_ctor_var_arr(Node *node, Var vars[], int cur_index)
{
    if (!node || cur_index > SIZE_OF_VARS_ARR)
    {
        return cur_index;
    }
    
    if (node->type == VAR && check_replay(vars, cur_index, node->value.var_value))
    {   
        strncpy(vars[cur_index].var_name, node->value.var_value, MAX_LEN_VALUE);
        vars[cur_index].index = cur_index;
        cur_index++;
    }

    if (node->l_son)
        cur_index = lang_ctor_var_arr(node->l_son, vars, cur_index);
    if (node->r_son)
        cur_index = lang_ctor_var_arr(node->r_son, vars, cur_index);

    return cur_index;
}

static int _write_node_(Node *node, int cur_var_arr_index, FILE *ASM_file)
{
    if (!node)
        return 0;

    Mode_of_print mode = (node->type == LOG) ? INORDER : POSTORDER;

    if (node->r_son)
        _write_node_(node->r_son, cur_var_arr_index, ASM_file);

    if (mode == INORDER)
        _print_to_file_(node, cur_var_arr_index, ASM_file);

    if (node->l_son)
        _write_node_(node->l_son, cur_var_arr_index, ASM_file);

    if (mode == POSTORDER)
        _print_to_file_(node, cur_var_arr_index, ASM_file);

    return 0;
}


static int _print_to_file_(Node *node, int cur_var_arr_index, FILE *ASM_file)
{   
    int push_flag = 1;
    if (node->parent && node->parent->type == LOG
        && node->parent->value.log_op == ASG && node->pos == LEFT)
    {
        push_flag = 0;
    }

    switch(node->type)
    {
        case NUM:
        {
            Print_num(node->value.dbl_value, ASM_file);
            break;
        }
        case VAR:
        {
            Print_var(node->value.var_value, cur_var_arr_index, ASM_file, push_flag);
            break;
        }
        case OP:
        {
            Print_op(node->value.op_value, ASM_file);
            break;
        }
        case LOG:
        {
            Print_log_op(node->value.log_op, ASM_file);
            break;
        }
    }

    return 0;
}

static int  Print_num(double value, FILE *ASM_file)
{
    fprintf(ASM_file, "PUSH %g\n", value);
    return 0;
}

static int  Print_var(char var[], int cur_var_arr_index, FILE *ASM_file, int push_flag)
{
    int index = _find_var_index_(var, cur_var_arr_index);
    fprintf(ASM_file, "%s[%d]\n",(push_flag == 1) ? "PUSH " : "",index);

    return 0;
}

static int _find_var_index_(char var[], int cur_var_arr_index)
{
    for (int counter = 0; counter < cur_var_arr_index; counter++)
    {
        if (strcmp(var, vars[counter].var_name) == 0)
            return vars[counter].index;
    }

    return -1;
}

static int  Print_op(Arith_Operation arith_op, FILE *ASM_file)
{
    switch(arith_op)
    {
        case ADD:
        {
            fprintf(ASM_file, "ADD\n");
            break;
        }
    }

    return 0;
}

static int  Print_log_op(Log_Oper log_op, FILE *ASM_file)
{
    switch(log_op)
    {
        case PRINT:
        {
            fprintf(ASM_file, "OUT\n");
            break;
        }
        case ASG:
        {
            fprintf(ASM_file, "POP ");
            break;
        }
    }

    return 0;
}

int find_len_log_op(Log_Oper log_op)
{
    switch (log_op)
    {
    case ASG:
        return 1;
    case IF:
        return 2;
    case PRINT:
        return 5;
    }

    return 0;
}