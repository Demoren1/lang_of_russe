#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <lang.h>

static FILE *ASM_FILE = NULL;
static Node *label_num = 0;
static int cur_index = 0;

static Node *translate (Node *node);

static Node *handle_func (Node *node);

static Node *handle_empty(Node *node);

static Node *handle_funcall(Node *node);

static Node *handle_node(Node *node);

static Node *handle_num(Node *node);

static Node *handle_log(Node *node);

static Node *handle_var(Node *node);

int lang_trans(Node *root, const char *src)
{
    ASM_FILE = fopen(src, "w");
    fprintf(ASM_FILE, "JMP :main \n\n");

    translate(root);

    fprintf(ASM_FILE, "HLT \n\n");
    fclose(ASM_FILE);
    return 0;   
}

static Node *translate (Node *node)
{
    if (!node)
        return 0;

    if (node->l_son)
        handle_func(node->l_son);

    if (node->r_son)
        translate(node->r_son);


    return 0;
}

static Node *handle_func (Node *node)
{
    if (!node)
    {
        return 0;
    }

    if (node->type == LOG &&
        node->value.log_op == DEF)
    {
        fprintf(ASM_FILE, "%s: \n", node->l_son->value.var_value);
        handle_func(node->l_son);
    }

    if (node->l_son)
    {
        // todo add args handle
    }

    if (node->r_son)
    {
        handle_empty(node->r_son);
    }

    return 0;
}

static Node *handle_empty(Node *node)
{   
    if (!node)
    {
        return 0;
    }

    if (node->l_son)
    {   
        handle_node(node->l_son);
    }

    if (node->r_son)
    {
        handle_empty(node->r_son);
    }

    return 0;
}

static Node *handle_node(Node *node)
{   
    if (node->type == LOG &&
        node->value.log_op == ASG)
    {
        handle_node(node->r_son);
        fprintf(ASM_FILE, "POP [%d]\n", cur_index++);
    }

    else if (node->type == NUM)
    {
        fprintf(ASM_FILE, "PUSH %g\n", node->value.dbl_value);
    }

    return node;
}

Node *handle_num(Node *node)
{
    fprintf(ASM_FILE, "PUSH %g", node->value.dbl_value);

    return node;
}

static Node *handle_funcall(Node *node)
{
    return 0;
}