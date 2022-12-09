#ifndef LANG_GUARD
#define LANG_GUARD

#include <tree_funcs.h>
#include <diff_funcs.h>
const int vars_shift = 1024;

const int STANDART_N_TOKENS = 1024;

typedef struct Token_t
{
    Type_of_expression type_node;
    Value value;
} Token;

typedef struct Tokens_t
{
    Token **tokens;
    size_t size;
    int cur_pos;
}Tokens;

int cpu();

int asm_func(int n_cmds, char *cmds[]);

Node *diff();

int lang_dtor_nodes(Node *nodes[], size_t lines);

int lang_to_new_line(char buffer[]);

int lang_write_asm(Node *nodes[], size_t lines, const char *path_asm);

int lang_ctor_var_arr(Node *node, Var *var_arr, int cur_index);

int find_len_log_op(Log_Oper log_op);

int lekser_handle(Tokens *data_tokens, Buffer *buff);

double lekser_NUM(char *str, size_t *shift);

Arith_Operation lekser_OP(char *str, size_t *shift);

Log_Oper lekser_LOG_OP(char *str, size_t *shift);

char *lekser_VAR(char *str, size_t *shift);

Tokens *lekser_data_tokens_ctor(Tokens *data_tokens);

int lekser_dtor(Tokens *data_tokens);

#endif