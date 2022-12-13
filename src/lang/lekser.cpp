#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <lang.h>

Tokens *lekser_data_tokens_ctor(Tokens *data_tokens)
{
    SOFT_ASS_NO_RET(data_tokens == NULL);

    data_tokens->tokens = (Token **) calloc(STANDART_N_TOKENS, sizeof(Token*));
    data_tokens->cur_pos = 0;
    data_tokens->size    = 0;

    return data_tokens;
}

Token *token_ctor(Type_of_expression type, Value value)
{
    Token *token = (Token *) calloc(1, sizeof(Token*));
    
    token->type_node = type;
    token->value     = value;

    SOFT_ASS_NO_RET(token == NULL);
    return token;
}

int lekser_handle(Tokens *data_tokens, Buffer *buff)
{
    SOFT_ASS_NO_RET(data_tokens == NULL);
    SOFT_ASS_NO_RET(buff == NULL);

    int index = 0;
    size_t shift = 0;

    for (; index < buff->size; index++)
    {   
        
        while(index < buff->size && (isspace(buff->buffer[index]) || buff->buffer[index] == '\0')) 
            index++;
        
        if (index >= buff->size )
            break;
        
        // printf("cur char = %c\n", buff->buffer[index]);
        
        shift = 0;
        Value value = {};
        Token *cur_token = token_ctor(NOT_TYPE, value);

        if (isdigit(buff->buffer[index]))
        {
            cur_token->type_node = NUM;
            cur_token->value.dbl_value = lekser_NUM(buff->buffer + index, &shift);
        }
        else if(cur_token->type_node != NUM && lekser_OP(buff->buffer + index, &shift) != NOT_OP)
        {
            
            cur_token->value.op_value = lekser_OP(buff->buffer + index, &shift);
            if (cur_token->value.op_value != NOT_OP)
            {   
                
                cur_token->type_node = ARITHM_OP;
            }
        }
        else if (cur_token->type_node != ARITHM_OP  && 
                 cur_token->type_node != NUM        && 
                lekser_LOG_OP(buff->buffer + index, &shift) != NOT_LOG_OP)
        {
            cur_token->value.log_op = lekser_LOG_OP(buff->buffer + index, &shift);
            if (cur_token->value.log_op != NOT_LOG_OP)
                cur_token->type_node = LOG;
        }
        else if (cur_token->type_node != LOG &&
                 cur_token->type_node != ARITHM_OP && 
                 cur_token->type_node != NUM &&
                 lekser_SEP(buff->buffer + index, &shift) != NOT_SEP)
        {
            cur_token->value.sep = lekser_SEP(buff->buffer + index, &shift);
            if (cur_token->value.sep != NOT_SEP)
                cur_token->type_node  = SEP;
        }
        else
        {
            
            // printf("cur char is %c\n", buff->buffer[index]);
            // printf("token type = %d\n", cur_token->type_node);
            cur_token->type_node = VAR;
            cur_token->value.var_value = lekser_VAR(buff->buffer + index, &shift);
        }

        
        // printf("shift = %zd \n", shift);
        data_tokens->tokens[data_tokens->cur_pos] = cur_token;
        data_tokens->cur_pos++;
        // printf("before index = %d\n", index);
        index += shift;
        // printf("after index = %d\n", index);

        
    }
    
    data_tokens->size = data_tokens->cur_pos;
    data_tokens->cur_pos = 0;
    
    return 0;
}

double lekser_NUM(char *str, size_t *shift)
{
    double tmp_double = 0;

    char *end_str = NULL;
    tmp_double = strtod(str, &end_str);

    *shift = end_str - str;

    return tmp_double;
}

#define DEF_ARITH_CMD(arith_op, code, naive_name, custom_name)    \
    else if (strstr(str, custom_name) == str)               \
    {                                                       \
        *shift = strlen(custom_name);                       \
        return arith_op;                                    \
    }                               

Arith_Operation lekser_OP(char *str, size_t *shift)
{
    char *end_str = NULL;

    if(0 == 1);
    #include <operation.h>
    
    return NOT_OP;
}

#define DEF_SEP(sep_name, code, naive_name, custom_name)    \
    else if (strstr(str, custom_name) == str)               \
    {                                                       \
        *shift = strlen(custom_name);                       \
        return sep_name;                                    \
    } 

Separators lekser_SEP(char *str, size_t *shift)
{
    
    if (0 == 1);
    #include <separators.h>

    
    return NOT_SEP;
}
#undef DEF_SEP

#undef DEF_ARITH_CMD

#define DEF_LOG_OP_CMD(log_op, code, naive_name, custom_name)           \
    else if ((end_str = strstr(str, custom_name)) == str)               \
    {   *shift = strlen(custom_name);                                   \
        return log_op;                                                  \
    }

Log_Oper lekser_LOG_OP(char *str, size_t *shift)
{
    char *end_str = 0;
    if(1 == 0);
   
   #include <log_operation.h>

    return NOT_LOG_OP;
}

#undef DEF_LOG_OP_CMD

char *lekser_VAR(char *str, size_t *shift)
{
    char *tmp_str = strtok(str, " ");
    char *to_ret_str = strdup(tmp_str);

    *shift = strlen(to_ret_str);

    return to_ret_str;
}

int lekser_dtor(Tokens *data_tokens)
{
    if (data_tokens == NULL)
        return -1;

    for (int index = 0; index < data_tokens->size; index++)
    {
        Token *cur_token = data_tokens->tokens[index];
        if (cur_token->type_node == VAR)
            free(cur_token->value.var_value);
            
        else if (cur_token->type_node == FUNC)
            free(cur_token->value.var_value);

        free(cur_token);
    }
    return 0;
}