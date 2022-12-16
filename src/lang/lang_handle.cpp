#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <general_debug.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <diff_funcs.h>
#include <lang.h>

int lang_add_spaces(Buffer *buff)
{
    char *new_buff = (char *) calloc(2*buff->size, sizeof(char));

    for (int index = 0; index < buff->size; index++)
    {
        new_buff[2*index]   = buff->buffer[index];
        new_buff[2*index+1] = ' ';
    }
    free(buff->buffer);
    buff->buffer = new_buff;
    buff->size *= 2;
    return 0;
}

int lang_get_name(char src[], int n_names, char **arr_with_names)
{
    if (n_names > 2)
    {
        strncpy(src, *(arr_with_names + 1), MAX_LEN_VALUE);
    }
    else 
        strncpy(src, "expression.txt", MAX_LEN_VALUE);

    return 0;
}