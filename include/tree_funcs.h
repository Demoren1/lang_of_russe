#ifndef TREE_GUARD
#define TREE_GUARD
#include <math.h>

const int LEN_OF_DATA = 256;

const int SIZE_OF_VARS_ARR = 256;

#define MAX_LEN_VALUE 256

#define FUNC_GENERAL_INFO(object)  __FILE__, __FUNCTION__, #object, __LINE__

#define TREE_CTOR(tree) tree_ctor(&list, FUNC_GENERAL_INFO(tree))

#define POISON "POISON"

typedef enum 
{   
    NOT_SIDE    = -1,
    LEFT        = 1,
    RIGHT       = 2
}Pos_of_node;

typedef enum 
{
    NOT_TYPE  = -1,
    ARITHM_OP = 1,
    VAR       = 2,
    NUM       = 3, 
    LOG       = 4,
    SEP       = 5,
    FUNC      = 6,
    EMPTY     = 7
} Type_of_expression;


#define DEF_SEP(sep_name, code, naive_name, custom_name)    \
    sep_name = code,

typedef enum 
{
    #include <separators.h>
} Separators;

#undef DEF_SEP

#define DEF_LOG_OP_CMD(log_op, code, naive_name, custom_name, ...)       \
    log_op = code,

typedef enum
{
    #include <log_operation.h>
    MAX_LOG_CMD
} Log_Oper;

#undef DEF_LOG_OP_CMD

#define DEF_ARITH_CMD(arith_op, code, naive_name, custovm_name)  \
           arith_op = code,                     

typedef enum 
{   
    #include <operation.h>
    MAX_CMD_OP
} Arith_Operation;

#undef DEF_ARITH_CMD

union Value
{
    double dbl_value;
    Arith_Operation op_value;
    char *var_value = NULL;
    Log_Oper log_op;
    int staff_val;
    Separators sep;
};

typedef enum 
{   
    ERROR_PRIOR     = -999,
    LOG_PRIOR       = 999,
    SEP_PRIOR       = 999,
    EMPTY_PRIOR     = 999,
    FUNC_PRIOR      = 999,
    NUM_PRIOR       = 1,
    VAR_PRIOR       = 1,
    ADD_PRIOR       = 1,
    SUB_PRIOR       = 1,
    MUL_PRIOR       = 2,
    DIV_PRIOR       = 2,
    DEGREE_PRIOR    = 5,
    UNAR_OP_PRIOR   = 10
} Priorities;

typedef struct Node_t
{    
    Type_of_expression type;

    Value value;

    Node_t *parent;
    Node_t *l_son;
    Node_t *r_son;
    Pos_of_node pos;

    Priorities priority;

} Node;

typedef struct Var_t
{
    char var_name[MAX_LEN_VALUE];
    double var_value = NAN;
    int index = -1;
} Arg_elem;

typedef struct Func_t
{
    char func_name[MAX_LEN_VALUE];
    double var_value = NAN;
    int index = -1;
} Func_elem;

typedef struct Tree_t
{
    Node* root;
} Tree;

int tree_ctor(Tree *tree);

Node *node_ctor();

Node* node_connect(Node *parent, Node *node, Pos_of_node pos);

Node *node_disconnect (Node *parent, Pos_of_node pos, int dtor_flag);

Node* node_ctor_connect(Node *parent, Pos_of_node pos);

int node_dtor(Node *node);

int node_dtor_calloc_data(Node *node, const char* buffer, int size);

int tree_show_graph();

Node* node_copy_node(Node *node);

int node_copy_data(Node *new_node, Node *old_node);

int node_switch_sons(Node *node);


#endif



