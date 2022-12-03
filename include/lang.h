#ifndef LANG_GUARD
#define LANG_GUARD

int cpu();

int asm_func(int n_cmds, char *cmds[]);

Node *diff();

int lang_dtor_nodes(Node *nodes[], size_t lines);

int lang_to_new_line(char buffer[]);

#endif