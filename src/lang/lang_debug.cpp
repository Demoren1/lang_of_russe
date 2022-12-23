#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include <lekser_debug.h>

static char *find_type(Type_of_expression type);

static char *find_arith_op(Arith_Operation op);

static char *find_log_op(Log_Oper op);

static char *find_sep(Separators sep);

static char to_ret[MAX_LEN_VALUE] = {};

int lekser_dump(Tokens *data_tokens)
{

    FILE *lekser_dump = fopen("lekser_dump.txt", "w");
    fprintf(lekser_dump, "\tTYPE \t\t\t VALUE\n");
    for (int i = 0; i < data_tokens->size; i++)
    {
        
        Token *cur_token = data_tokens->tokens[i];
        
        fprintf(lekser_dump, "[%3d] : %d %-16s ", i, cur_token->type_node, find_type(cur_token->type_node));
        switch(cur_token->type_node)
        {
            case NUM:
            {
                fprintf(lekser_dump, "%g \n", cur_token->value.dbl_value);
                break;
            }
            case ARITHM_OP:
            {
                fprintf(lekser_dump, "%s \n", find_arith_op(cur_token->value.op_value));
                break;
            }
            case LOG:
            {
                fprintf(lekser_dump, "%s \n", find_log_op(cur_token->value.log_op));
                break;
            }
            case SEP:
            {
                fprintf(lekser_dump, "%s \n", find_sep(cur_token->value.sep));
                break;
            }
            case FUNC:
            {
                fprintf(lekser_dump, "%s \n", cur_token->value.var_value);
                break;
            }
            case VAR:
            {
                fprintf(lekser_dump, "%s \n", cur_token->value.var_value);
                break;
            }
            default:
                fprintf(lekser_dump, "ERROR\n");
        }
    }

    fclose(lekser_dump);
    return 0;
}

#define DEF_ARITH_CMD(arith_op, code, naive_name, custom_name)  \
    case arith_op:                                              \
        return strncpy(to_ret, naive_name,MAX_LEN_VALUE);

static char *find_arith_op(Arith_Operation op)
{
    switch (op)
    {
        #include <operation.h>

        default:
            return strncpy(to_ret, "ERROR", MAX_LEN_VALUE);
    }
}

#undef DEF_ARITH_CMD

#define DEF_LOG_OP_CMD(log_op, code, naive_name, custom_name, ...)    \
    case log_op:                                                 \
    {                                                               \
        return strncpy(to_ret, naive_name, MAX_LEN_VALUE);        \
    }

static char *find_log_op(Log_Oper op)
{

    switch (op)
    {
        #include <log_operation.h>

        default: return strncpy(to_ret, "ERROR", MAX_LEN_VALUE);
    }
}
#undef DEF_LOG_OP_CMD

#define DEF_SEP(sep_name, code, naive_name, custom_name)    \
    case sep_name:                                              \
        return strncpy(to_ret, naive_name, MAX_LEN_VALUE);

static char *find_sep(Separators sep)
{
    switch (sep)
    {
        #include <separators.h>

        default: return strncpy(to_ret, "ERROR SEP", MAX_LEN_VALUE);
    }
}

#undef DEF_SEP

static char *find_type(Type_of_expression type)
{
    switch(type)
    {
        case NOT_TYPE:
            return strncpy(to_ret, "NOT_TYPE", MAX_LEN_VALUE);
        case NUM:
            return strncpy(to_ret, "NUM", MAX_LEN_VALUE);
        case VAR:
            return strncpy(to_ret, "VAR", MAX_LEN_VALUE);
        case ARITHM_OP:
            return strncpy(to_ret, "ARITHM_OP", MAX_LEN_VALUE);
        case SEP:
            return strncpy(to_ret, "SEP", MAX_LEN_VALUE);
        case LOG:
            return strncpy(to_ret, "LOG_OP", MAX_LEN_VALUE);
        case FUNC:
            return strncpy(to_ret, "FUNC", MAX_LEN_VALUE);
        default:
            return strncpy(to_ret, "ERROR", MAX_LEN_VALUE);
    }
}