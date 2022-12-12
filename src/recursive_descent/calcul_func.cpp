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

Tokens *Data_tokens = NULL;

Node* get_General(Tokens *data_tokens)
{
    Data_tokens = data_tokens;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    Node* node = get_Expression();

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    assert(Data_tokens->cur_pos == Data_tokens->size);

    return node;
}

Node *get_VAR()
{
    Node *node = 0;
    int index = 0;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    node = Create_VAR_node(cur_token->value.var_value);
    // printf("var is %s\n", node->value.var_value);  
    SOFT_ASS_NO_RET(node == NULL);

    Data_tokens->cur_pos++;
    return node;
}

Node* get_Num()
{
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    Node *node = Create_NUM_node(cur_token->value.dbl_value);
    Data_tokens->cur_pos++;
    return node;

}
Node* get_P()
{
    Node *node = 0;
    Node *value_node = 0;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    if (cur_token->type_node == ARITHM_OP &&
        cur_token->value.op_value != NOT_OP)
    {
        Node *op_node = get_UNAR_OP();
        node = op_node;
    }

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    if (cur_token->value.sep == OPEN_CIRC)
    {
        Data_tokens->cur_pos++;
        value_node = get_Expression();
        
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        assert (cur_token->value.sep == CLOSE_CIRC);
        Data_tokens->cur_pos++;
    }
    else if (cur_token->type_node == VAR)
    {
        value_node = get_VAR();
    }
    else if (cur_token->type_node == NUM)
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
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    Node *node = Create_OP_node(cur_token->value.op_value);
    Data_tokens->cur_pos++;

    return node;
}

Node* get_T()
{
    Node *node = 0;
    node = get_Degree();

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    while(Data_tokens->cur_pos < Data_tokens->size &&
          (cur_token->value.op_value == MUL || 
           cur_token->value.op_value == DIV))
    {
        Arith_Operation op = cur_token->value.op_value;

        Data_tokens->cur_pos++;
        Node* r_node = get_Degree();
        
        if (op == MUL)
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

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    while( Data_tokens->cur_pos < Data_tokens->size &&
         (cur_token->value.op_value == ADD || 
          cur_token->value.op_value == SUB))
    {
        Arith_Operation op = cur_token->value.op_value;

        Data_tokens->cur_pos++;
        Node* r_node = get_T();
        
        if (op == ADD)
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
    DBG;
    node = get_P();
    DBG;

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    while(Data_tokens->cur_pos < Data_tokens->size &&
          cur_token->value.op_value == DEGREE)
    {
        Data_tokens->cur_pos++;

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

#if 0
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
#endif