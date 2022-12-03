#include <stdio.h>
#include <stdalign.h>   
#include <general_debug.h>
#include <tree_funcs.h>

int lang_to_new_line(char buffer[])
{
    int num_sym = 0;

    while(buffer[num_sym] != '\0')
    {
        num_sym++;
    }
    num_sym++;

    return num_sym;
}


int lang_dtor_nodes(Node *nodes[], size_t lines)
{
    for (int i = 0; i < lines; i++)
    {
        node_dtor(nodes[i]);
    }

    return 0;
}