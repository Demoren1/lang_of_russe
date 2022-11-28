#include <stdio.h>
#include <assert.h>
#include "../include/stack_objects.h"

FILE* open_with_no_buff(const char* name_file, const char* regime)
{
    FILE* file_ptr = fopen(name_file, regime);
    assert(file_ptr != NULL);

    setvbuf(file_ptr, NULL, _IONBF, 0);
    
    return file_ptr;
}

