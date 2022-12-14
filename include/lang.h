#ifndef LANG_GUARD
#define LANG_GUARD

#include <tree_funcs.h>
#include <diff_funcs.h>

#define WRITE_ASM(str, ...)                     \
                                                \
        fprintf(ASM_FILE, str, ##__VA_ARGS__);

const int vars_shift = 1024;

const int STANDART_N_TOKENS = 1024;

const int SHIFT_IN_ARGS = 64;

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

int lekser_handle(Tokens *data_tokens, Buffer *buff);

double lekser_NUM(char *str, size_t *shift);

Arith_Operation lekser_OP(char *str, size_t *shift);

Log_Oper lekser_LOG_OP(char *str, size_t *shift);

char *lekser_VAR(char *str, size_t *shift);

Separators lekser_SEP(char *str, size_t *shift);

Tokens *lekser_data_tokens_ctor(Tokens *data_tokens);

int lekser_dtor(Tokens *data_tokens);

int lang_trans(Node *root, const char *src);

int handle_internal_args(Node *node, Arg_elem internal_args[]);

int handle_general_args(Node *node, Arg_elem general_args[]);

int add_internal_args_to_general(Arg_elem internal_args[], Arg_elem general_args[]);

int add_to_args(Arg_elem *args, char var_value[]);

#endif