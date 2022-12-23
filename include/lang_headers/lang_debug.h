#ifndef LANG_DEBUG_GUARD
#define LANG_DEBUG_GUARD

#include <lang.h>
#include <tree_funcs.h>


#define CHECK_TOKENS()  do {                                                                                                \
                        printf("prev token type %d\n"                                                                       \
                               "cur token type  %d\n"                                                                       \
                               "next token type %d\n", Data_tokens->tokens[Data_tokens->cur_pos-1]->type_node,               \
                                Data_tokens->tokens[Data_tokens->cur_pos]->type_node,                                        \
                                Data_tokens->tokens[Data_tokens->cur_pos + 1]->type_node);                                   \
                        DBG;                                                                                                \
                        }while (0);



#endif 