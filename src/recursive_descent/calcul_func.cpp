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
#include <lang_debug.h>

static int check_end();                 //todo add operations with func

Tokens *Data_tokens = NULL;

Node* get_General(Tokens *data_tokens)
{
    Data_tokens = data_tokens;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    Node* node = Create_EMPTY_node();

    get_Tree(node);
    
    return node;
}

int get_Tree(Node *node)
{
    Node *left_node  = get_ASS();
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    // SOFT_ASS(cur_token->type_node != SEP || cur_token->value.sep != CLOSE_FIG)

    if (Data_tokens->cur_pos >= Data_tokens->size || 
        (cur_token->type_node == SEP && cur_token->value.sep == CLOSE_FIG))
    {   
        if (left_node)
        {
            node_connect(node, left_node, LEFT);
        }
        
        return 0;
    }
    
    while (cur_token->type_node == SEP)    
    {
        Data_tokens->cur_pos++;
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

    Node *right_node = Create_EMPTY_node();

    get_Tree(right_node);

    node_connect(node, right_node, RIGHT);
    node_connect(node, left_node, LEFT);
    
    return 0;
}

Node *get_ASS()
{
    Node *new_node = NULL;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    while (Data_tokens->cur_pos < Data_tokens->size && cur_token->type_node == SEP)
    {
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        
        if (cur_token->type_node == SEP && cur_token->value.sep == CLOSE_FIG)
        {
            break;
        }
        Data_tokens->cur_pos++;
    }

    if (Data_tokens->cur_pos >= Data_tokens->size || 
       (cur_token->type_node == SEP && cur_token->value.sep == CLOSE_FIG))
    {      
        return 0;
    }

    Token *next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];
    Token *prev_token = Data_tokens->tokens[Data_tokens->cur_pos - 1];

    if (Data_tokens->size - Data_tokens->cur_pos > 0 && 
        next_token->type_node == LOG && 
        next_token->value.log_op == ASG)
    {
        new_node = Create_LOG_OP_node(ASG);
        Node *l_node = Create_VAR_node(cur_token->value.var_value);
        
        Data_tokens->cur_pos += 2 ;
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];

        Node *r_node = NULL;
        r_node = get_ASS();

        // if (cur_token->type_node == VAR && next_token->type_node == SEP)
        // {
        //     r_node = get_LOG();            
        // }
        // else
        // { 
        //     r_node = get_Expression();
        // }

        node_connect(new_node, l_node, LEFT);
        node_connect(new_node, r_node, RIGHT);
    }
    else if (cur_token->type_node == LOG        || 
            (next_token->type_node == SEP       && 
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
    Token *next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];

    Node *keyword_node = NULL; 
    Node *func_node  = NULL;

    if (cur_token->type_node == LOG && cur_token->value.log_op == DEF)
    {
        func_node = handle_DEF();
        keyword_node = Create_LOG_OP_node(DEF);
        node_connect(keyword_node, func_node, LEFT);
    }
    else if (cur_token->type_node == VAR)
    {
        keyword_node = Create_LOG_OP_node(FUNCALL);
        func_node = Create_VAR_node(cur_token->value.var_value);
        func_node->type = FUNC;
        cur_token->type_node = FUNC;
        node_connect(keyword_node, func_node, LEFT);

        Data_tokens->cur_pos++;
    }
    else if (cur_token->type_node == LOG)
    {
        keyword_node = Create_LOG_OP_node(cur_token->value.log_op);
        Data_tokens->cur_pos++;
    }

    Node *expr_node  = NULL;

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    if (Data_tokens->cur_pos < Data_tokens->size &&
        (cur_token->type_node == VAR        || 
         cur_token->type_node == NUM        ||
         cur_token->type_node == ARITHM_OP  ||
         cur_token->type_node == LOG))
    {   
        if (cur_token->type_node != LOG || cur_token->value.log_op != ELSE)
        {   
            func_node = get_Func(keyword_node);
        }
    }
    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    // SOFT_ASS_NO_RET(cur_token->type_node != SEP);
    if (cur_token->type_node == SEP && cur_token->value.sep == OPEN_CIRC)  
    {
        get_LOG_handle_CIRC(expr_node, func_node, keyword_node);
    }

    if (Data_tokens->cur_pos >= Data_tokens->size || 
        Data_tokens->tokens[Data_tokens->cur_pos]->type_node != SEP)
    {
        return keyword_node;
    }
    Node *block_node = NULL;
    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    if (cur_token->type_node == SEP &&
        cur_token->value.sep == OPEN_FIG)
    {
        
        get_LOG_handle_FIG(expr_node, func_node, keyword_node, block_node);
    }

    return keyword_node;
}

Node* handle_DEF()
{
    Data_tokens->cur_pos++;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    Node *func_name = Create_VAR_node(cur_token->value.var_value);
    func_name->type = FUNC;
    cur_token->type_node = FUNC;

    Data_tokens->cur_pos++;

    return func_name;
}

int get_LOG_handle_CIRC(Node *expr_node, Node *func_node, Node *keyword_node)
{
    Data_tokens->cur_pos++;
    expr_node = get_ASS();

    SOFT_ASS_NO_RET(!expr_node);

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    Node *new_node1 = expr_node;
    Node *new_node2 = NULL;
    while(cur_token->type_node == SEP &&
            cur_token->value.sep != CLOSE_CIRC)
    {   
        Data_tokens->cur_pos++;
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

        if (Data_tokens->cur_pos >= Data_tokens->size) 
            break;
        
        Node *new_node2 = get_ASS();
        SOFT_ASS_NO_RET(!new_node1);
        SOFT_ASS_NO_RET(!new_node2);

        node_connect(new_node1, new_node2, LEFT);

        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        new_node1 = new_node2;
    }
    

    if (func_node)
    {
        SOFT_ASS_NO_RET(!func_node);
        SOFT_ASS_NO_RET(!expr_node);
        node_connect(func_node, expr_node, LEFT);
    }
    else
    {
        SOFT_ASS_NO_RET(!keyword_node);
        SOFT_ASS_NO_RET(!expr_node);
        
        node_connect(keyword_node, expr_node, LEFT);
    }
    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    SOFT_ASS_NO_RET(cur_token->value.sep != CLOSE_CIRC && cur_token->value.sep != CLOSE_FIG);
    Data_tokens->cur_pos++;

    return 0;
}

int get_LOG_handle_FIG(Node *expr_node, Node *func_node, Node *keyword_node, Node *block_node)
{
    block_node = Create_EMPTY_node();

    Data_tokens->cur_pos++;
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    
    get_Tree(block_node);

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    Token *next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];
    
    if (func_node)
    {
        node_connect(func_node, block_node, RIGHT);   
    }
    else
    {    
        if (Data_tokens->size - Data_tokens->cur_pos > 3    &&
            next_token->type_node == LOG                    &&
            next_token->value.log_op == ELSE)
        {   
            Data_tokens->cur_pos += 3;
    
            Node *block_for_else = Create_EMPTY_node();
            Node *general_node = Create_EMPTY_node();
            
            get_Tree(block_for_else);

            SOFT_ASS_NO_RET(!keyword_node);
            SOFT_ASS_NO_RET(!block_node);
            SOFT_ASS_NO_RET(!general_node);
            SOFT_ASS_NO_RET(!block_for_else);

            node_connect(keyword_node, general_node, RIGHT);
            node_connect(general_node, block_node, LEFT);
            node_connect(general_node, block_for_else, RIGHT);
            
        }
        else
        {   
            SOFT_ASS_NO_RET(!keyword_node);
            SOFT_ASS_NO_RET(!block_node);

            node_connect(keyword_node, block_node, RIGHT);
        }
    }

    if ((Data_tokens->cur_pos) >= Data_tokens->size || 
        Data_tokens->tokens[Data_tokens->cur_pos]->type_node != SEP)
    {
        return 0;
    }

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    Token *prev_token = Data_tokens->tokens[Data_tokens->cur_pos - 1];

    SOFT_ASS_NO_RET(cur_token->value.sep != CLOSE_FIG && cur_token->value.sep != CLOSE_CIRC);

    Data_tokens->cur_pos++;
    
    return 0;
}


Node *get_Func(Node *keyword_node)
{   
    Node *func_node  = NULL;
    Token *prev_token = Data_tokens->tokens[Data_tokens->cur_pos - 1];
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    Token *next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];
    
    func_node = get_ASS();
    
    cur_token->type_node = FUNC;
    func_node->type = FUNC; 

    SOFT_ASS_NO_RET(!keyword_node)
    SOFT_ASS_NO_RET(!func_node)
    
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

        tmp_node2 = get_ASS();
        
        SOFT_ASS_NO_RET(!tmp_node1);
        SOFT_ASS_NO_RET(!tmp_node2);

        node_connect(tmp_node1, tmp_node2, LEFT);
        tmp_node1 = tmp_node2;

        Data_tokens->cur_pos++;
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

    if (cur_token->type_node == SEP && cur_token->value.sep == CLOSE_CIRC)
        Data_tokens->cur_pos++;

    return func_node;
}

Node *get_VAR()
{
    Node *node = 0;

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];    
    node = Create_VAR_node(cur_token->value.var_value);
    SOFT_ASS_NO_RET(node == NULL);

    Data_tokens->cur_pos++;

    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
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
    Node *node = NULL;
    Node *value_node = NULL;

    if (Data_tokens->cur_pos >= Data_tokens->size)
    {     
        return node;
    }

    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    if (cur_token->type_node == ARITHM_OP &&
        cur_token->value.op_value != NOT_OP)
    {
        Node *op_node = get_UNAR_OP();
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
        Token *prev_token = Data_tokens->tokens[Data_tokens->cur_pos-1];
        
        // if (cur_token->type_node == SEP && 
        //     cur_token->value.sep == CLOSE_CIRC &&
        //     prev_token->type_node != SEP)
        // {
        //     
        //     Data_tokens->cur_pos++;
        // }

        node = op_node;
        return node;
    }
    
    else if (cur_token->type_node == SEP &&
        cur_token->value.sep == OPEN_CIRC)
    {   
        Data_tokens->cur_pos++;
        value_node = get_ASS();
    
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
    Token *next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];

    Node *node = Create_OP_node(cur_token->value.op_value);
    Data_tokens->cur_pos++;

    Token *prev_token = cur_token;
    cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    next_token = Data_tokens->tokens[Data_tokens->cur_pos + 1];
    
    if (prev_token->type_node == ARITHM_OP && 
        (prev_token->value.op_value == SIN ||
         prev_token->value.op_value == COS ||
         prev_token->value.op_value == LN  ||
         prev_token->value.op_value == TG  ||
         prev_token->value.op_value == ABS ))
    {   
        Data_tokens->cur_pos++;
        Node *tmp_node = get_ASS();

        SOFT_ASS_NO_RET(!node);
        SOFT_ASS_NO_RET(!tmp_node);
        node_connect(node, tmp_node, RIGHT);

        prev_token = Data_tokens->tokens[Data_tokens->cur_pos-1];
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

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
           cur_token->value.op_value == ZDIV || 
           cur_token->value.op_value == DIV))
    {
        
        Arith_Operation op = cur_token->value.op_value;

        Data_tokens->cur_pos++;
        Node* r_node = get_ASS();
        
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
        else if (op == ZDIV)
        {
            Node *new_node = Create_OP_node(ZDIV);
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
          cur_token->value.op_value == SUB ||
          cur_token->value.op_value == GEQ ||
          cur_token->value.op_value == GE  ||
          cur_token->value.op_value == LEQ ||
          cur_token->value.op_value == LE))
    {
        Arith_Operation op = cur_token->value.op_value;

        Data_tokens->cur_pos++;


        Node* r_node = get_ASS();
        Node *new_node = NULL;

        switch (op)
        {
            case ADD:
                new_node = Create_OP_node(ADD);
                break;
            case SUB:
                new_node = Create_OP_node(SUB);
                break;
            case GEQ:
                new_node = Create_OP_node(GEQ);
                break;
            case GE:
                new_node = Create_OP_node(GE);
                break;
            case LEQ:
                new_node = Create_OP_node(LEQ);
                break;
            case LE:
                new_node = Create_OP_node(LE);
                break;
        }
        node_connect(new_node, node, LEFT);
        node_connect(new_node, r_node, RIGHT);
        node = new_node;
        
        cur_token = Data_tokens->tokens[Data_tokens->cur_pos];
    }

    return node;
}

Node* get_Degree()
{
    Node *node = 0;
    
    node = get_P();
    Token *cur_token = Data_tokens->tokens[Data_tokens->cur_pos];

    if (check_end())
        return node;
    

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
       Data_tokens->tokens[Data_tokens->cur_pos]->type_node == SEP )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}