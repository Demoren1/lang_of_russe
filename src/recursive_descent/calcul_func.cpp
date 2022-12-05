#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <general_debug.h>
#include <dsl.h>
#include <calcul_funcs.h>
#include <lang.h>

const char* STR = NULL;

Node* get_General(const char *str)
{
    STR = str;
    
    Node* node = get_LOG_OP();

    assert(*STR == '\0');

    return node;
}

Node *get_VAR()
{
    Node *node = 0;
    char var_str[MAX_LEN_VALUE] = {};
    int index = 0;
    
    while(*STR != '\0' && isalpha(*STR))
    {
        var_str[index] = *STR;
        index++;
        STR++;
    }
    
    node = Create_VAR_node(var_str);
    // printf("var is %s\n", node->value.var_value);  
    SOFT_ASS_NO_RET(node == NULL);
    return node;
}

Node* get_Num()
{
    double val = 0;
    int shift = 0;
    const char *old_s = STR;

    if (isdigit(*STR))
    {   
        char *end_str = NULL;
        val = strtod(STR, &end_str);

        if (end_str != NULL)
            STR = end_str;
    }

    STR += shift;
    assert(STR != old_s);

    Node *node = Create_NUM_node((double) val);
    return node;

}
Node* get_P()
{
    Node *node = 0;
    Node *value_node = 0;
    
    if (diff_get_operation(STR) != NOT_OP)
    {
        Node *op_node = get_UNAR_OP();
        node = op_node;
    }
    if (*STR == '(')
    {
        STR++;
        value_node = get_Expression();
        assert (*STR == ')');
        STR++;
    }
    else if (isalpha(*STR) && diff_get_log_operation(STR) == NOT_LOG_OP)
    {
        value_node = get_VAR();
    }
    else if (isdigit(*STR))
    {
        value_node = get_Num();
    }

    if (node)
        node_connect(node, value_node, RIGHT);
    else 
        node = value_node;
    
    return node;
}

Node *get_UNAR_OP()
{   
    Node *node = Create_OP_node(diff_get_operation(STR));

    while(*STR != '\0' && isalpha(*STR))
    {   
        STR++;  
    }

    return node;
}

Node* get_T()
{
    Node *node = 0;
    node = get_Degree();
    
    while(*STR == '*' || *STR == '/')
    {
        int op = *STR;

        STR++;
        Node* r_node = get_Degree();
        
        if (op == '*')
        {
            Node *new_node = Create_OP_node(MUL);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
        else 
        {
            Node *new_node = Create_OP_node(DIV);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
    }

    return node;
}

Node* get_Expression()
{
    Node* node = 0;
    
    node = get_T();
    
    while(*STR == '+' || *STR == '-')
    {
        int op = *STR;

        STR++;
        Node* r_node = get_T();
        
        if (op == '+')
        {
            Node *new_node = Create_OP_node(ADD);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
         
        else 
        {
            Node *new_node = Create_OP_node(SUB);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
    }

    return node;
}

Node* get_Degree()
{
    Node *node = 0;
    node = get_P();

    while(*STR == '^')
    {
        STR++;
        Node *r_node = get_P();

        {
            Node *new_node = Create_OP_node(DEGREE);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
    }

    return node;
}


Node *get_LOG_OP()
{
    Node* node = 0;
    
    node = get_Expression();
    
    while(diff_get_log_operation(STR) != NOT_LOG_OP || *STR == '(')
    {
        Log_Oper log_op = diff_get_log_operation(STR);
        int shift = find_len_log_op(log_op);

        STR += shift;

        Node* r_node = get_Expression();
        
        switch(log_op)
        {
            case ASG:
            {
                Node *new_node = Create_LOG_OP_node(ASG);
                node_connect(new_node, node, LEFT);
                node_connect(new_node, r_node, RIGHT);
                node = new_node;
                break;
            }
            case IF:
            {
                Node *new_node = Create_LOG_OP_node(IF);
                node_connect(new_node, r_node, RIGHT);
                node = new_node;
                break;
            }
            case PRINT:
            {
                Node *new_node = Create_LOG_OP_node(PRINT);
                node_connect(new_node, r_node, RIGHT);
                node = new_node;
                break;
            }
        }
    }
    
    return node;
}