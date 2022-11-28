#include <stdio.h>
#include <string.h>

int transfer_char()
{
    char str[256] ={};

    fgets(str, 256, stdin);
    
    int len = strlen(str);
    printf("PUSH 0      ##%s", str);

    for (int i = len - 1; i >= 0; i--)
    {
        printf("PUSH %d\n", str[i]);
    }

    for (int i = 0; i < len + 1; i++)
    {
        printf("POP [%d]\n", i);
    }
    printf("SHOW\n");

    return 0;
}