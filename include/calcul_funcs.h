#ifndef CALC_GUARD
#define CALC_GUARD

#include <tree_funcs.h>
#include <lang.h>

Node *get_General(Tokens *data_tokens);

Node *get_Expression();

Node *get_T();

Node *get_P();

Node *get_Num();

Node *get_Degree();

Node *get_VAR();

Node *get_UNAR_OP();

int get_Tree(Node *node);

Node *get_LOG();

Node *get_ASS();

Node *get_Block();

Node *get_Func(Node *node);

#endif
