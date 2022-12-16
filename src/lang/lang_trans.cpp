#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <lang.h>
#include <trans.h>


static FILE *ASM_FILE = NULL;
static Node *label_num = 0;
static int else_index = 0;
static int if_index = 0;
static int start_arg_index = 10240;
static int start_internal_args = 14336; 

int lang_trans(Node *root, const char *src)
{
    ASM_FILE = fopen(src, "w");

    WRITE_ASM("push %d\n" "pop VAR_IN_FUNCALL\n", start_internal_args);
    WRITE_ASM("call :main \n");
    WRITE_ASM("hlt \n\n");
    translate(root);

    fclose(ASM_FILE);
    return 0;   
}

Node *translate (Node *node)
{
    if (!node)
        return 0;

    if (node->l_son->type == LOG &&
        node->l_son->value.log_op == DEF)
    {
        
        handle_func(node->l_son->l_son);
        WRITE_ASM("RET\n\n");
    }

    if (node->r_son)
    {
        start_arg_index += SHIFT_IN_ARGS;                      
        translate(node->r_son);
    }

    return 0;
}

Node *handle_func (Node *node)
{
    if (!node)
    {
        return 0;
    }

    Arg_elem general_args[MAX_LEN_VALUE] = {};          //general args in FUNC!!!
    Arg_elem internal_args[MAX_LEN_VALUE] = {};         // args from foo ( arg1 ; arg2 )

    if (node->type == FUNC)
    {
        fprintf(ASM_FILE, "%s: \n", node->value.var_value);
        WRITE_ASM("push %d\npop VAR_IN_DEF\n\n", start_arg_index);

        if (node->l_son)
        {
            handle_internal_args(node->l_son, internal_args);
        }

        add_internal_args_to_general(internal_args, general_args);
        handle_general_args(node->r_son, general_args);
    }

    #if 0
    int i = 0;
    while(general_args[i].index != -1)
    {
        printf("[%d]cur var is %s\n", i, general_args[i].var_name);

        for (int index = 0; index < strlen(general_args[i].var_name); index++)
            printf("%d = %c\n", general_args[i].var_name[index], general_args[i].var_name[index]);
        i++;
    }
    #endif

    if (node->r_son)
    {
        handle_empty(node->r_son, general_args);
    }

    return 0;
}

int add_internal_args_to_general(Arg_elem internal_args[], Arg_elem general_args[])
{
    int tmp_index = 0;

    while(internal_args[tmp_index].index != -1)
    {
        add_to_args(general_args, internal_args[tmp_index].var_name);
        tmp_index++;
    }

    return 0;
}

int handle_general_args(Node *node, Arg_elem general_args[])
{
    if (node->type == VAR)
    {
        add_to_args(general_args, node->value.var_value);
    }

    if (node->l_son)
        handle_general_args(node->l_son, general_args);
    if (node->r_son)
        handle_general_args(node->r_son, general_args);

    return 0;
}


int handle_internal_args(Node *node, Arg_elem internal_args[])
{
    Node *tmp_node = node;
    int index = 0;
    while (tmp_node)
    {   
        index = add_to_args(internal_args, tmp_node->value.var_value);
        WRITE_ASM("push [VAR_IN_FUNCALL + %d] \n", index);
        WRITE_ASM("pop [VAR_IN_DEF + %d] \n", index++);
        tmp_node = tmp_node->l_son;
    }
    WRITE_ASM("\n");

    return 0;
}

int add_to_args(Arg_elem *args, char var_value[])
{
    int new_index = 0;

    while(args[new_index].index != -1)
    {   

        if (strcasecmp(var_value, args[new_index].var_name) == 0)
        {
            return 0;
        }
        new_index++;
    }

    args[new_index].index = new_index;
    strncpy(args[new_index].var_name, var_value, MAX_LEN_VALUE);

    return new_index;
}

Node *handle_empty(Node *node, Arg_elem general_args[])
{   
    if (!node)
    {
        return 0;
    }

    if (node->l_son)
    {   
        handle_node(node->l_son, general_args);
    }

    if (node->r_son)
    {
        handle_empty(node->r_son, general_args);
    }

    return 0;
}

Node *handle_node(Node *node, Arg_elem general_args[])
{   
    if (!node)
        return 0;

    switch(node->type)
    {
        case NUM:
        {
            handle_num(node);
            break;
        }
        case VAR:
        {
            handle_var(node, general_args);
            break;
        }
        case ARITHM_OP:
        {
            handle_arithm(node, general_args);
            break;
        }
        case LOG:
        {
            handle_log(node, general_args);
            break;
        }
        case EMPTY:
        {
            handle_empty(node, general_args);
            break;
        }
    }

    return node;
}

Node *handle_num(Node *node)
{
    fprintf(ASM_FILE, "push %g\n", node->value.dbl_value);

    return node;
}

Node *handle_var(Node *node, Arg_elem general_args[])
{
    WRITE_ASM("push [VAR_IN_DEF + %d]\n", find_index(general_args, node->value.var_value));

    return 0;
}

int find_index(Arg_elem general_args[],char var_value[])
{
    for (int counter = 0; general_args[counter].index != -1; counter++ )
    {
        if (strcasecmp(general_args[counter].var_name, var_value) == 0)
        {
            return   (general_args[counter].index);
        }
    }

    return 0;
}

#define DEF_ARITH_CMD(arith_op, code, naive_name, custovm_name) \
    case arith_op:                                              \
    {                                                           \
        WRITE_ASM("%s\n", #arith_op);                           \
        break;                                                  \
    }

Node *handle_arithm(Node *node, Arg_elem general_args[])
{   
    handle_node(node->l_son, general_args);
    handle_node(node->r_son, general_args);

    switch(node->value.op_value)
    {
        #include <operation.h>

        default:
            printf("ERROR");
            break;
        
    }

    return 0;
}

Node *handle_log(Node *node, Arg_elem general_args[])
{
    switch(node->value.log_op)
    {
        case ASG:
        {
            handle_ASG(node, general_args);
            break;
        }
        case IF:
        {
            handle_IF(node, general_args);
            break;
        }
        case PRINT:
        {
            handle_PRINT(node, general_args);
            break;
        }
        case PRINT_STR:
        {
            handle_PRINT_STR(node, general_args);
            break;
        }
        case WHILE:
        {
            handle_WHILE(node, general_args);
            break;
        }
        case FUNCALL:
        {
            handle_FUNCALL(node, general_args);
            break;
        }
        case RETURN:
        {
            handle_RETURN(node, general_args);
            break;
        }
        case INPUT:
        {
            handle_INPUT(node, general_args);
            break;
        }
        default:
            printf("ERROR");
    }

    return 0;
}

int handle_ASG(Node *node, Arg_elem general_args[])
{

    handle_node(node->r_son, general_args);

    SOFT_ASS_NO_RET(node->l_son->type != VAR);
    WRITE_ASM("pop [VAR_IN_DEF + %d]\n\n", find_index(general_args, node->l_son->value.var_value));

    return 0;
}

int handle_IF(Node *node, Arg_elem general_args[])
{
    WRITE_ASM("push 0\n");
    handle_node(node->l_son, general_args);

    WRITE_ASM("jee :else_%d\n", else_index);
    handle_empty(node->r_son->l_son, general_args);

    WRITE_ASM("\njmp :endif_%d\n", if_index);
    WRITE_ASM("else_%d:\n", else_index++);

    handle_empty(node->r_son->r_son, general_args);
    WRITE_ASM("endif_%d:\n", if_index++);

    return 0;   
}

int handle_WHILE(Node *node, Arg_elem general_args[])
{
    WRITE_ASM("while_%d: \n", if_index);
    WRITE_ASM("push 0 \n");
    handle_node(node->l_son, general_args);
    WRITE_ASM("jee :end_while_%d \n", else_index);
    
    handle_empty(node->r_son, general_args);

    WRITE_ASM("jmp :while_%d \n", if_index++);
    WRITE_ASM("end_while_%d: \n", else_index++);

    return 0;
}

int handle_INPUT(Node *node, Arg_elem general_args[])
{
    Node *tmp_node = node->l_son;

    while (tmp_node)
    {   
        WRITE_ASM("in\n")
        WRITE_ASM("pop [VAR_IN_DEF + %d]\n\n", find_index(general_args, node->l_son->value.var_value));
        // handle_node(tmp_node, general_args);
        tmp_node = tmp_node->l_son;
    }

    return 0;
}

int handle_PRINT(Node *node, Arg_elem general_args[])
{
    Node *tmp_node = node->l_son;

    do{ 
        handle_node(tmp_node, general_args);
        WRITE_ASM("out\n\n");
        tmp_node = tmp_node->l_son;
    }
    while (tmp_node != NULL && (tmp_node->type != LOG && tmp_node->type != ARITHM_OP));

    return 0;
}

int handle_PRINT_STR(Node *node, Arg_elem general_args[])
{
    char str[MAX_LEN_VALUE] = {};

    strncpy(str, node->l_son->value.var_value, MAX_LEN_VALUE);

    int len = strlen(str);
    WRITE_ASM("push 0      ## %s\n", str);
    WRITE_ASM("push 10 \n");


    for (int i = len - 1; i >= 0; i--)
    {
        WRITE_ASM("push %d\n", str[i]);
    }

    for (int i = 0; i < len + 2; i++)
    {
        WRITE_ASM("pop [%d]\n", i);
    }
    WRITE_ASM("show\n\n");

    return 0;
}

int handle_FUNCALL(Node *node, Arg_elem general_args[])
{
    Node *tmp_node = node->l_son->l_son;
    int index = 0;
    while (tmp_node)
    {   
        WRITE_ASM("push [VAR_IN_DEF + %d] \n", find_index(general_args, tmp_node->value.var_value));
        WRITE_ASM("pop [VAR_IN_FUNCALL + %d] \n", index++);
        tmp_node = tmp_node->l_son;
    }
    WRITE_ASM("\n");
    WRITE_ASM("call :%s\n", node->l_son->value.var_value);
    WRITE_ASM("push RCX\n\n");
    return 0;
}

int handle_RETURN(Node *node, Arg_elem general_args[])
{   
    handle_node(node->l_son, general_args);
    WRITE_ASM("pop RCX\n\n");

    return 0;
}
