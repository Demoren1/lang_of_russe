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
        
        while(index < buff->size && isspace(buff->buffer[index])) 
            index++;
        shift = 0;
        Value value = {};
        Token *cur_token = token_ctor(NOT_TYPE, value);
        printf("cur char = %c\n", buff->buffer[index]);

        if (isdigit(buff->buffer[index]))
        {
            cur_token->type_node = NUM;
            cur_token->value.dbl_value = lekser_NUM(buff->buffer + index, &shift);
        }
        else if(cur_token->type_node != NUM)
        {
            
            cur_token->type_node = ARITHM_OP;
            cur_token->value.op_value = lekser_OP(buff->buffer + index, &shift);
        }
        else if(cur_token->type_node != NUM && 
                cur_token->type_node != ARITHM_OP)
        {
            
            cur_token->type_node = LOG;
            cur_token->value.log_op = lekser_LOG_OP(buff->buffer + index, &shift);
        }
        else if(cur_token->type_node != NUM && 
                cur_token->type_node != ARITHM_OP && 
                cur_token->type_node != LOG)
        {
            DBG;
            cur_token->type_node = VAR;
            strncpy(cur_token->value.var_value, lekser_VAR(buff->buffer + index, &shift), MAX_LEN_VALUE);
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

Arith_Operation lekser_OP(char *str, size_t *shift)
{
    char *op_sym = str;
    char *end_str = NULL;

    if ((end_str = strstr(str, "ln")) == str)
    {
        *shift = end_str - str;
        return LN;
    }
    else if ((end_str = strstr(str, "sin")) == str)
    {
        *shift = end_str - str;
        return SIN;
    }
    else if ((end_str = strstr(str, "cos")) == str)
    {
        *shift = end_str - str;
        return COS;
    }
    else if ((end_str = strstr(str, "tg")) == str)
    {
        *shift = end_str - str;
        return TG;
    }

    switch (*op_sym)
    {
        case '+':
            *shift = 1;
            return ADD;
        case '-':
            *shift = 1;
            return SUB;
        case '*':
            *shift = 1;
            return MUL;
        case '/':
            *shift = 1;
            return DIV;
        case '^':
            *shift = 1;
            return DEGREE;   
    }
    
    return NOT_OP;
}

Log_Oper lekser_LOG_OP(char *str, size_t *shift)
{
    char *end_str = 0;

    if ((end_str = strstr(str, "=")) == str)
    {   *shift = end_str  - str;
        return ASG;
    }
    else if ((end_str = strstr(str, "if")) == str)
    {   *shift = end_str  - str;
        return IF;
    }
    else if ((end_str =  strstr(str, "print")) == str)
    {
        *shift = end_str  - str;
        return PRINT;
    }

    return NOT_LOG_OP;
}

char *lekser_VAR(char *str, size_t *shift)
{

    char *tmp_var = strtok(str, " ");
    char *ret_var = strdup(tmp_var);

    *shift = strlen(ret_var);
    
    return tmp_var;
}

int lekser_dtor(Tokens *data_tokens)
{
    if (data_tokens == NULL)
        return -1;

    for (int index = 0; index < data_tokens->size; index++)
    {
        Token *cur_token = data_tokens->tokens[index];
        free(cur_token);
    }

    return 0;
}