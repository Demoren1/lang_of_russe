#ifndef TRANS_GUARD
#define TRANS_GUARD

#include <tree_funcs.h>

Node *translate (Node *node);

Node *handle_func (Node *node);

Node *handle_empty(Node *node, Arg_elem general_args[]);

Node *handle_node(Node *node, Arg_elem general_args[]);

Node *handle_num(Node *node);

Node *handle_var(Node *node, Arg_elem general_args[]);

Node *handle_arithm(Node *node, Arg_elem general_args[]);

Node *handle_log(Node *node, Arg_elem general_args[]);

int find_index(Arg_elem general_args[],char var_value[]);

int handle_ASG(Node *node, Arg_elem general_args[]);

int handle_IF(Node *node, Arg_elem general_args[]);

int handle_ELSE(Node *node, Arg_elem general_args[]);

int handle_FUNCALL(Node *node, Arg_elem general_args[]);

int handle_PRINT(Node *node, Arg_elem general_args[]);

int handle_WHILE(Node *node, Arg_elem general_args[]);

int handle_INPUT(Node *node, Arg_elem general_args[]);

#endif
