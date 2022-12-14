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

static int check_end();

Tokens *Data_tokens = NULL;

Node* get_General(Tokens *data_tokens)
{
    Data_tokens = data_tokens;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    Node* node = Create_EMPTY_node();

    get_Tree(node);
    
    cur_token = Data_tokens->tokens[Data_tokens->cur_pos - 1];
    // assert(Data_tokens->cur_pos == Data_tokens->size);
    return node;

}

int get_Tree(Node *node)
{
    Node *left_node  = get_ASS();
    
    // printf("cur pos %d, size %zd \n", Data_tokens->cur_pos, Data_tokens->size);
    
    if (check_end())
    {   
        
        node_connect(node, left_node, LEFT);
        
        return 0;
    }
    
    Node *right_node = Create_EMPTY_node();
    
    get_Tree(right_node);
    
    node_connect(node, left_node, LEFT);
    node_connect(node, right_node, RIGHT);

    return 0;
}

Node *get_ASS()
{
    Node *new_node = NULL;
    
    if (check_end()) 
        return 0;

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    Token *next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];

    if (Data_tokens->size - Data_tokens->cur_pos > 0 && 
        next_token->type_node == LOG && 
        next_token->value.log_op == ASG)
    {
        new_node = Create_LOG_OP_node(ASG);
        Node *l_node = Create_VAR_node(cur_token->value.var_value);
        
        Data_tokens->cur_pos += 2 ;
        Node *r_node = get_Expression();

        node_connect(new_node, l_node, LEFT);
        node_connect(new_node, r_node, RIGHT);
    }
    else if (cur_token->type_node == LOG || 
            (next_token->type_node == SEP && 
            next_token->value.sep == OPEN_CIRC))
    {
        new_node = get_LOG();   
    }
    else
    {   
        new_node = get_Expression();
    }

    return new_node;
}

Node *get_LOG()
{
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    Node *keyword_node = NULL; 

    if (cur_token->type_node == VAR)
    {
        keyword_node = Create_LOG_OP_node(FUNCALL);
    }
    else 
    {
        keyword_node = Create_LOG_OP_node(cur_token->value.log_op);
        Data_tokens->cur_pos++;
    }

    Node *expr_node  = NULL;
    Node *func_node  = NULL;

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    func_node = get_Func(keyword_node);

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    // SOFT_ASS_NO_RET(cur_token->type_node != SEP);
    if (cur_token->value.sep == OPEN_CIRC)  
    {
        Data_tokens->cur_pos++;
        expr_node = get_Expression();

        if (func_node)
            node_connect(func_node, expr_node, LEFT);
        else
            node_connect(keyword_node, expr_node, LEFT);

        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        // SOFT_ASS_NO_RET(cur_token->value.sep != CLOSE_CIRC)
        Data_tokens->cur_pos++;
    }
        
    if (Data_tokens->cur_pos == Data_tokens->size || 
        Data_tokens->tokens[Data_tokens->cur_pos]->type_node != SEP) 
        return keyword_node;


    Node *block_node = NULL;
    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    // printf("cur token val %d \n", cur_token->value.sep);

    if (cur_token->type_node == SEP &&
        cur_token->value.sep == OPEN_FIG)
    {
        block_node = Create_EMPTY_node();
        Data_tokens->cur_pos++;
        
        get_Tree(block_node);

        if (func_node)
            node_connect(func_node, block_node, RIGHT);
        else
            node_connect(keyword_node, block_node, RIGHT);

        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        // SOFT_ASS_NO_RET(cur_token->value.sep != CLOSE_FIG);
    
        Data_tokens->cur_pos++;
    }
    
    return keyword_node;
}

Node *get_Func(Node *keyword_node)
{   
    Node *func_node  = NULL;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    if (Data_tokens->cur_pos < Data_tokens->size &&
        cur_token->type_node == VAR )
    {
        cur_token->type_node = FUNC;
        
        func_node = Create_VAR_node(cur_token->value.var_value);
        func_node->type = FUNC;
        node_connect(keyword_node, func_node, LEFT);        
        Data_tokens->cur_pos++;   
                                                                    
        Node *tmp_node1 = func_node;
        Node *tmp_node2 = NULL;
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

        while(cur_token->type_node == SEP && (cur_token->value.sep == COLON_SEP || cur_token->value.sep == OPEN_CIRC))
        {
            Data_tokens->cur_pos++;
            cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
            if (cur_token->type_node == SEP && cur_token->value.sep == CLOSE_CIRC)
                break;

            tmp_node2 = Create_VAR_node(cur_token->value.var_value);

            node_connect(tmp_node1, tmp_node2, LEFT);
            tmp_node1 = tmp_node2;

            Data_tokens->cur_pos++;
            cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        }

        if (cur_token->type_node == SEP && cur_token->value.sep == CLOSE_CIRC)
            Data_tokens->cur_pos++;
    }

    return func_node;

}

Node *get_VAR()
{
    Node *node = 0;

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];    
    node = Create_VAR_node(cur_token->value.var_value);
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
    // printf("before cur pos is %d\n", Data_tokens->cur_pos);

    Node *node = 0;
    Node *value_node = 0;
        
    if (check_end())
    {     
        return node;
    }

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    // printf("cur_token value %d\n", cur_token->value.op_value);

    if (cur_token->type_node == ARITHM_OP &&
        cur_token->value.op_value != NOT_OP)
    {
        Node *op_node = get_UNAR_OP();
        node = op_node;
    }
    
    else if (cur_token->type_node == SEP &&
        cur_token->value.sep == OPEN_CIRC)
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

    // printf("value_node %p \n", value_node);
    // printf("after cur pos is %d\n", Data_tokens->cur_pos);

    if (node && value_node)
    {
        node_connect(node, value_node, RIGHT);
    }
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

    if (check_end())
        return node;
    
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    while(Data_tokens->cur_pos < Data_tokens->size &&
          cur_token->type_node == ARITHM_OP        &&
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
        else if (op == DIV)
        {
            Node *new_node = Create_OP_node(DIV);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

    return node;
}

Node* get_Expression()
{
    Node* node = 0;  
    node = get_T();

    if (check_end())
    {
        return node;
    }

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    while( Data_tokens->cur_pos < Data_tokens->size &&
          cur_token->type_node      == ARITHM_OP    &&
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
         
        else if (op == SUB)
        {
            Node *new_node = Create_OP_node(SUB);
            node_connect(new_node, node, LEFT);
            node_connect(new_node, r_node, RIGHT);
            node = new_node;
        }

        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

    return node;
}

Node* get_Degree()
{
    Node *node = 0;
    
    node = get_P();

    if (check_end())
        return node;
    
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    while(Data_tokens->cur_pos < Data_tokens->size &&
          cur_token->type_node      == ARITHM_OP   &&
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
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

    return node;
}

static int check_end()
{
    if (Data_tokens->cur_pos >= Data_tokens->size || 
        Data_tokens->tokens[Data_tokens->cur_pos]->type_node == SEP)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}