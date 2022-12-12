#include <stdio.h>
#include <math.h>
#include <string.h>
#include <tree_funcs.h>
#include <diff_funcs.h>
#include <dsl.h>


Node *Create_OP_node(Arith_Operation oper_value)
{                
    Node *new_node = node_ctor();                             
    new_node->value.op_value = oper_value;              
    new_node->type = ARITHM_OP;                                
    new_node->priority = find_op_priority(oper_value);

    return new_node;
}

Node *Create_SEP_node(Separators sep)
{                
    Node *new_node = node_ctor();                             
    new_node->value.sep = sep;              
    new_node->type = SEP;                                
    new_node->priority = SEP_PRIOR;

    return new_node;
}

Node *Create_NUM_node(double dbl_value)
{
    Node *new_node = node_ctor();                             
    new_node->value.dbl_value = dbl_value;              
    new_node->type = NUM;                                
    new_node->priority = NUM_PRIOR;

    return new_node;
}

Node *Create_VAR_node(char var[])
{
    Node *new_node = node_ctor(); 
    // new_node->value.var_value = (char *) calloc(MAX_LEN_VALUE, sizeof(char));
    
    new_node->value.var_value = strdup(var);

    // strncpy(new_node->value.var_value, var, MAX_LEN_VALUE);              
    
    new_node->type = VAR;                                
    new_node->priority = VAR_PRIOR;

    return new_node;
}

Node *Create_LOG_OP_node(Log_Oper log_op)
{
    Node *new_node = node_ctor();
    new_node->value.log_op = log_op;
    new_node->type = LOG;
    new_node->priority = LOG_PRIOR;

    return new_node;
}

Node *ADD_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(ADD);}

Node *SUB_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(SUB);}

Node *MUL_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(MUL);}

Node *DIV_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(DIV);}

Node *DEGREE_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(DEGREE);}

Node *LN_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(LN);}

Node *COS_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(COS);}

Node *SIN_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(SIN);}

Node *TG_OP(Node *l_node, Node *r_node)
{LINK_WITH_OP(TG);}

Node *Diff_ADD(Node *node, Arith_Operation oper_value, char var_for_diff[])
{
    return ADD_OP(DL, DR);
}

Node *Diff_MUL(Node *node, Arith_Operation oper_value, char var_for_diff[])
{
    return ADD_OP(MUL_OP(DL, CR), MUL_OP(CL, DR));
}

Node *Diff_DIV(Node *node, Arith_Operation oper_value, char var_for_diff[])
{   
    
    return DIV_OP(SUB_OP(MUL_OP(DL, CR), MUL_OP(DR, CL)), MUL_OP(CR, CR));
}

Node *Diff_DEGREE(Node *node, char var_for_diff[])
{   
    return MUL_OP(DEGREE_OP(CL, CR),ADD_OP(MUL_OP(DR, LN_OP(NULL, CL)), MUL_OP(MUL_OP(DIV_OP(Create_NUM_node(1), CL), DL), CR)));
}
 
Node *Diff_LN(Node *node, char var_for_diff[])
{
    return MUL_OP(DIV_OP(Create_NUM_node(1), CR), DR);
}

Node* Diff_SIN(Node *node, char var_for_diff[])
{
    return MUL_OP(COS_OP(NULL, CR), DR);
}

Node* Diff_COS(Node *node, char var_for_diff[])
{
    return MUL_OP( MUL_OP(Create_NUM_node(-1),SIN_OP(NULL, CR)), DR);
}

Node *Diff_TG(Node *node, char var_for_diff[])
{
    return MUL_OP(DIV_OP(Create_NUM_node(1), DEGREE_OP(COS_OP(NULL, CR), Create_NUM_node(2))), DR);
}

#if 0

#endif 