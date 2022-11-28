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

static int check_replay(Var var_arr[], int cur_index, char var_name);

static int replace_var_on_num(Node *node, char var_name, double var_value);

static int wrap_equivalents(Node *node);

static Pos_of_node find_zero_side(Node *node);

static Pos_of_node find_one_side(Node *node);

static int kill_zero_side(Pos_of_node zero_side, Node *node);

static int kill_one_side(Pos_of_node zero_side, Node *node);

static char* del_new_line_and_spaces(Buffer *buffer);

static int input_val_type(Node *node, Buffer *buff);

static int create_node_if_need(char cur_sym, Node* node, Buffer *buff);

static int exit_node_if_need(char cur_sym, Buffer *buff);

static int switch_sons_if_need(Node *node);

static double count_num(Node *node, Node *l_son, Node *r_son);

static Node *diff_node_ctor(Type_of_expression type, const void *value);

static int len_op(const char *operation);

static int compute_constants(Node *node);

static FILE* SRC = 0;

int diff_handle_src(const char* path_to_file, Buffer *buff)
{   
    SOFT_ASS(path_to_file == NULL);
    SOFT_ASS(buff == NULL);
    
    SRC = fopen(path_to_file, "rw+");

    SOFT_ASS(SRC == NULL);

    buff->size = diff_size_for_buffer(SRC, path_to_file);

    buff->buffer = (char*) calloc(buff->size, sizeof(char));        
    SOFT_ASS(buff->buffer == NULL);

    int test_fread = 0;
    test_fread = fread(buff->buffer, sizeof(char), buff->size, SRC);      
    SOFT_ASS(test_fread == 0);  

    buff->buffer = del_new_line_and_spaces(buff);

    buff->buffer[buff->size] = '\0';
    buff->curr_index = 0;

    return 0;
}

int diff_size_for_buffer(FILE *SRC, const char *path_to_file)
{
    SOFT_ASS(SRC == NULL);
    SOFT_ASS(path_to_file == NULL);

    struct stat buf ={};
    stat(path_to_file, &buf);

    return buf.st_size + 1;
}

static char* del_new_line_and_spaces(Buffer* buff)
{
    SOFT_ASS_NO_RET(buff == NULL);
    SOFT_ASS_NO_RET(buff->buffer == NULL);

    char *new_buff = (char*) calloc(buff->size, sizeof(char));

    int index_buff = 0;
    int index_new_buff = 0;
    for (; index_buff < buff->size; index_buff++)
    {
        if (buff->buffer[index_buff] == '\n')
        {
            buff->buffer[index_buff] = '\0';
        }

        if (buff->buffer[index_buff] != ' ')
        {   
            new_buff[index_new_buff] = buff->buffer[index_buff];
            index_new_buff++;
        }
    }

    free(buff->buffer);
    return new_buff;
}

int diff_buff_dtor(Buffer *buffer)
{   
    if (SRC != NULL)
    {
        fclose(SRC);
        SRC = NULL;
    }
    if (buffer->buffer != NULL)
    {
        free(buffer->buffer);
        buffer->buffer = NULL;
    }  

    return 0;
}


int diff_simplify(Node *node)
{
    int changes = 1;

    TREE_DUMP(node, INORDER);
    while(changes > 0)
    {
        changes = 0;
        changes += compute_constants(node);
        changes += wrap_equivalents(node);
    }
    TREE_DUMP(node, INORDER);
    return changes;
}

int wrap_equivalents(Node *node)
{
    int changes = 0;
    if(!node)
        return changes;

    Pos_of_node zero_side = find_zero_side(node);    

    if (zero_side != NOT_SIDE)
    {
        changes++;
        kill_zero_side(zero_side, node);
    }
    
    Pos_of_node one_side = find_one_side(node);
    if (one_side != NOT_SIDE)
    {   
        changes += kill_one_side(one_side, node);
    }

    if (node->l_son && node->l_son->type == OP)
    {
        changes += wrap_equivalents(node->l_son);
    }

    if (node->r_son && node->r_son->type == OP)
    {   
        changes += wrap_equivalents(node->r_son);
    }

    return changes;
}

static int kill_one_side(Pos_of_node one_side, Node *node)
{  
    Node *one_node = 0;
    Node *conjugate_node = 0;
    Node *parent = node->parent;
    Node *tmp_node = node;
    int changes = 0;
    int fake_parent_flag = 0;

    if (one_side == LEFT)
    {
        one_node = NL;
        conjugate_node = NR;
    }
    else
    {
        one_node = NR;
        conjugate_node = NL;
    }

    if (conjugate_node == NULL)
        return changes;

    Operation oper = node->value.op_value;    
    
    if (!node->parent)
        fake_parent_flag = 1;
    
    if (!fake_parent_flag && node->pos > 0)
    {   
        node_disconnect(parent, node->pos, 0);
    }
    
    if  (oper == MUL || 
        (oper == DEGREE && one_side == RIGHT) ||
        (oper == DIV && one_side == RIGHT))
    {   
        changes++;        
        if (!fake_parent_flag)
        {
            
            node_connect(parent, node_copy_node(conjugate_node), node->pos);
            
        }
        else
        {
            tmp_node = node_copy_node(conjugate_node);
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);
            node_copy_data(node, tmp_node);
            
            node_dtor(tmp_node);
            return changes;
        }
    }
    else if(oper == DEGREE && one_side == LEFT)
    {
        changes++;
        if (!fake_parent_flag)
        {
            
            node_connect(parent, Create_NUM_node(1), node->pos);
        }
        else
        {
            tmp_node = Create_NUM_node(1);
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);
            node_copy_data(node, tmp_node);
            
            node_dtor(tmp_node);
            return changes;
        }
    }
    
    if (!fake_parent_flag && changes != 0)
    {
        node_dtor(node);
    }
    else if (node->pos > 0 && parent)
    {
        node_connect(parent, node, node->pos);   
    }
    
    return changes;    
}

static int kill_zero_side(Pos_of_node zero_side, Node *node)
{  
     
    Node *zero_node = 0;
    Node *conjugate_node = 0;
    Node *parent = node->parent;
    Node *tmp_node = node;
    int changes = 0;
    int fake_parent_flag = 0;

    if (zero_side == LEFT)
    {
        zero_node = NL;
        conjugate_node = NR;
    }
    else
    {
        zero_node = NR;
        conjugate_node = NL;
    }

    if (conjugate_node == NULL)
        return 0;
    
    Operation oper = node->value.op_value;    
    
    if (!node->parent)
        fake_parent_flag = 1;

    if (!fake_parent_flag && node->pos > 0)
    {   
        node_disconnect(parent, node->pos, 0);
    }

    if (oper == ADD || oper == SUB)
    {   
        changes++;
        if (!fake_parent_flag)
            node_connect(parent, node_copy_node(conjugate_node), node->pos);
        else
        {
            tmp_node = node_copy_node(conjugate_node);
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);
            
            node_copy_data(node, tmp_node);
            
            node_dtor(tmp_node);
            return changes;
        }
    }

    else if (oper == MUL || (zero_side == LEFT && oper == DEGREE) ||(zero_side == LEFT && oper == DIV))
    {   
        changes++;
        if (!fake_parent_flag)
            node_connect(parent, Create_NUM_node(0), node->pos);
        else
        {
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);

            node->type = NUM;
            node->value.op_value = NOT_OP;
            node->value.dbl_value = 0;
            node->priority = NUM_PRIOR;
        }
    }

    else if (zero_side == RIGHT && oper == DEGREE)
    {   
        changes++;
        if (!fake_parent_flag)
            node_connect(parent, Create_NUM_node(0), node->pos);
        else
        {
            node_disconnect(node, LEFT, 1);
            node_disconnect(node, RIGHT, 1);

            node->type = NUM;
            node->value.op_value = NOT_OP;
            node->value.dbl_value = 1;
            node->priority = NUM_PRIOR;
        }   
    }


    if (!fake_parent_flag && changes != 0)
    {
        node_dtor(node);
    }
    else if (node->pos > 0 && parent)
    {
        node_connect(parent, node, node->pos);
    }
    
    return changes++;    
}


static Pos_of_node find_one_side(Node *node)
{
    if (NL && NR && NL->type == NUM && NLV.dbl_value == 1)
        return LEFT;

    if (NR && NL && NR->type == NUM && NRV.dbl_value == 1)
    {   
        return RIGHT;
    }
    

    return NOT_SIDE; 
}

static Pos_of_node find_zero_side(Node *node)
{
    if (NL && NL->type == NUM && NLV.dbl_value == 0)
        return LEFT;

    if (NR && NR->type == NUM && NRV.dbl_value == 0)
        return RIGHT;

        
    return NOT_SIDE;
}

static int compute_constants(Node *node)
{   
    int changes = 0;
    if (node == NULL)
        return changes;

    double result = 0;
    
    if (!node->l_son && node->r_son && node->r_son->type == NUM)
    {
        count_num(node, NULL, node->r_son);
        changes++;
    }

    else if (node->l_son && node->r_son && node->l_son->type == NUM && node->r_son->type == NUM)
    {
        count_num(node, node->l_son, node->r_son);
        changes++;
    }

    if (node->l_son)
        changes += compute_constants(node->l_son);

    if (node ->r_son)
        changes += compute_constants(node->r_son);

    return changes;
}

int diff_ctor_var_arr(Node *node, Var *var_arr, int cur_index)
{
    if (!node && cur_index > MAX_VARS)
    {
        return cur_index;
    }
    
    if (node->type == VAR && check_replay(var_arr, cur_index, node->value.var_value))
    {   
        var_arr[cur_index].var_name = node->value.var_value;
        printf("%c = ", node->value.var_value);
        scanf("%lf", &var_arr[cur_index].var_value);
        cur_index++;
    }

    if (node->l_son)
        cur_index = diff_ctor_var_arr(node->l_son, var_arr, cur_index);
    if (node->r_son)
        cur_index = diff_ctor_var_arr(node->r_son, var_arr, cur_index);

    return cur_index;
}

static int check_replay(Var var_arr[], int cur_index, char var_name)
{   
    for (int index = 0; index < cur_index; index++)
    {
        if (var_name == var_arr[index].var_name)
            return 0;
    }

    return 1;
}

double diff_calc_tree(Node *node, Var var_arr[], int n_vars)
{
    Node* tmp_node = node_copy_node(node);
    for (int index = 0; index < n_vars && index < MAX_VARS; index++)
    {   
        replace_var_on_num(tmp_node, var_arr[index].var_name, var_arr[index].var_value);
    }

    diff_simplify(tmp_node);
    // open_log_pdf();
    double result = (fabs(tmp_node->value.dbl_value) > __DBL_EPSILON__) ? tmp_node->value.dbl_value : 0;
    TREE_DUMP(tmp_node, INORDER);
    node_dtor(tmp_node);

    return result;
}

static int replace_var_on_num(Node *node, char var_name, double var_value)
{
    if (!node)
        return 0;
    if (node->type == VAR && !isnan(var_value) && node->value.var_value == var_name)
    {
        node->value.dbl_value = var_value;
        node->value.var_value = 'E';
        node->priority = NUM_PRIOR;
        node->type = NUM;
    }

    if (node->l_son)
        replace_var_on_num(node->l_son, var_name, var_value);
    if (node->r_son)
        replace_var_on_num(node->r_son, var_name, var_value);

    return 0;
}

double diff_tailor_one_var(Node *node, int depth, char var_name, double var_value, double x0)
{
    Var var = {.var_name = var_name, .var_value = x0};
    Node *tmp_node1 = node_copy_node(node);
    Node *tmp_node2 = {};
    double delta_x0 = var_value - x0;
    double result = 0;
    double divider = 1;

    for (int level = 0; level < depth; level++)
    {
        diff_simplify(tmp_node1);
        TREE_DUMP(tmp_node1, INORDER);
        divider *= (level + (level == 0));
        double tmp_result = diff_calc_tree(tmp_node1, &var, 1);
        
        tmp_result *= pow(delta_x0, level) / divider;

        result += tmp_result;
        
        tmp_node2 = node_copy_node(tmp_node1);
        node_dtor(tmp_node1);
        
        tmp_node1 = diff_diff(tmp_node2, var_name);
        node_dtor(tmp_node2);
    }
    node_dtor(tmp_node1);

    return result;
}

static double count_num(Node *node, Node *l_son, Node *r_son)
{   
    double result = 0;


    if (l_son == NULL)
    {
        double value = r_son->value.dbl_value;

        switch(node->value.op_value)
        {
            case LN:
                result = log(value);
                break;
            case COS:
                result = cos(value);
                break;
            case SIN:
                result = sin(value);
                break;
            case TG:
                result = tan(value);
                break;
        }
    }
    
    else
    {   
        double l_value = l_son->value.dbl_value;
        double r_value = r_son->value.dbl_value;
        
        switch(node->value.op_value)
        {
            case ADD:
                result = l_value + r_value;
                break;
            case SUB:
                result = l_value - r_value;
                break;
            case MUL:  
                result = l_value * r_value;
                break;
            case DIV:
                if (r_value == 0)   break;
                result = l_value / r_value;
                break;
            case DEGREE:
                result = pow(l_value, r_value);
                break;
            
        }
    }

    if (node->r_son != NULL)
    {
        free(node->r_son);
        node->r_son = NULL;
    }
    
    if (node->l_son != NULL)
    {
        free(l_son);
        node->l_son = NULL;
    }

    node->type = NUM;
    node->value.op_value = NOT_OP;
    node->value.dbl_value = result;
    node->priority = NUM_PRIOR;

    return result;
}

static int switch_sons_if_need(Node *node)
{
    if (node->type != OP)
        return 0;

    switch (node->value.op_value)
    {
        case LN:
        case COS:
        case SIN:
        case TG:
        {
            node_switch_sons(node);
        }
    }

    return 0;
}

Node *diff_diff(Node *node, char var_for_diff)
{
    Type_of_expression type = node->type;
    Node *new_node = 0;
    switch(type)
    {
        case NUM:
        {   
            double value = 0;
            return Create_NUM_node(value);
        }
        case VAR:
        {   
            if (node->value.var_value == var_for_diff)            
                return Create_NUM_node(1);
            else
                return Create_NUM_node(0);
        }
        case OP:
        {
            
            switch (node->value.op_value)
            {
                case ADD:
                    return Diff_ADD(node, ADD, var_for_diff);
                case SUB:
                    return Diff_ADD(node, SUB, var_for_diff);
                case MUL:
                    return Diff_MUL(node, ADD, var_for_diff);
                case DIV:       
                    return Diff_DIV(node, SUB, var_for_diff); 
                case DEGREE:
                    return Diff_DEGREE(node, var_for_diff);
                case LN:
                    return Diff_LN(node, var_for_diff);
                case COS:
                    return Diff_COS(node, var_for_diff);
                case SIN:
                    return Diff_SIN(node, var_for_diff);
                case TG:
                    return Diff_TG(node, var_for_diff);
            }
            break;
        }
    }
    return 0;
}

Priorities find_op_priority(Operation operation)
{   
    switch(operation)
    {
        case ADD:
            return ADD_PRIOR;
        case SUB:
            return SUB_PRIOR;
        case MUL:
            return MUL_PRIOR;
        case DIV:
            return DIV_PRIOR;
        case DEGREE:
            return DEGREE_PRIOR;
        case LN:
        case COS:
        case SIN:
        case TG:
            return UNAR_OP_PRIOR;
        default:    
        {
            SOFT_ASS_NO_RET(1);
            return ERROR_PRIOR;
        }
    }

    return ERROR_PRIOR;
}

Operation diff_get_operation(const char *buff)
{
    char op_sym = buff[0];

    if (strstr(buff, "ln") == buff)
        return LN;
    else if (strstr(buff, "sin") == buff)
        return SIN;
    else if (strstr(buff, "cos") == buff)
        return COS;
    else if (strstr(buff, "tg") == buff)
        return TG;


    switch (op_sym)
    {
        case '+':
            return ADD;
        case '-':
            return SUB;
        case '*':
            return MUL;
        case '/':
            return DIV;
        case '^':
            return DEGREE;   
    }
    
    return NOT_OP;
}

