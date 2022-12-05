#ifndef LANG_GUARD
#define LANG_GUARD

const int vars_shift = 1024;

int cpu();

int asm_func(int n_cmds, char *cmds[]);

Node *diff();

int lang_dtor_nodes(Node *nodes[], size_t lines);

int lang_to_new_line(char buffer[]);

int lang_write_asm(Node *nodes[], size_t lines, const char *path_asm);

int lang_ctor_var_arr(Node *node, Var *var_arr, int cur_index);


#endif