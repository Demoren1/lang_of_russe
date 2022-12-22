DEF_LOG_OP_CMD(NOT_LOG_OP, -1, "ERROR", "ERROR", {})

DEF_LOG_OP_CMD(ASG, 1, "=", "=", 
{

    handle_node(node->r_son, general_args);

    SOFT_ASS_NO_RET(node->l_son->type != VAR);
    WRITE_ASM("pop [VID + %d]\n\n", find_index(general_args, node->l_son->value.var_value));

    return 0;
}
)

DEF_LOG_OP_CMD(IF, 2, "if", "if", 
{
    WRITE_ASM("push 0\n");
    handle_node(node->l_son, general_args);

    WRITE_ASM("jee :else_%d\n", else_index);
    
    if (node->r_son->l_son->type == EMPTY)
        handle_empty(node->r_son->l_son, general_args);
    else
        handle_empty(node->r_son, general_args);

    WRITE_ASM("\njmp :endif_%d\n", if_index);
    WRITE_ASM("else_%d:\n", else_index++);

    if (node->r_son->l_son->type == EMPTY)
        handle_empty(node->r_son->r_son, general_args);

    WRITE_ASM("endif_%d:\n", if_index++);

    return 0;   
}

)

DEF_LOG_OP_CMD(PRINT, 3, "print", "print", 
{
    Node *tmp_node = node->l_son;

    do{ 
        handle_node(tmp_node, general_args);
        WRITE_ASM("out\n\n");
        tmp_node = tmp_node->l_son;
    }
    while (tmp_node != NULL && (tmp_node->type != LOG && tmp_node->type != ARITHM_OP));

    return 0;
}
)

DEF_LOG_OP_CMD(WHILE, 4, "while", "while", 
{
    WRITE_ASM("while_%d: \n", if_index);
    WRITE_ASM("push 0 \n");
    handle_node(node->l_son, general_args);
    WRITE_ASM("jee :end_while_%d \n", else_index);
    
    handle_empty(node->r_son, general_args);

    WRITE_ASM("jmp :while_%d \n", if_index++);
    WRITE_ASM("end_while_%d: \n", else_index++);

    return 0;
}
)

DEF_LOG_OP_CMD(DEF, 5, "def", "def", {})

DEF_LOG_OP_CMD(FUNCALL, 6, "funcall", "funcall", 
{
    Node *tmp_node = node->l_son->l_son;
    int index = 0;
    while (tmp_node)
    {   
        WRITE_ASM("push [VID + %d] \n", find_index(general_args, tmp_node->value.var_value));
        WRITE_ASM("pop [VIF + %d] \n", index++);
        tmp_node = tmp_node->l_son;
    }
    WRITE_ASM("\n\n");
    
    WRITE_ASM("\npush VID\n");                    // right shift in RAM
    WRITE_ASM("dup\n");
    WRITE_ASM("dup\n");
    WRITE_ASM("push %d\n", SHIFT_IN_ARGS);
    WRITE_ASM("zdiv\n");
    WRITE_ASM("push %d\n", SHIFT_IN_ARGS);
    WRITE_ASM("mul\n");
    WRITE_ASM("sub\n");
    WRITE_ASM("sub\n");
    WRITE_ASM("push %d\n", SHIFT_IN_ARGS);
    WRITE_ASM("add\n");
    WRITE_ASM("pop VID\n");

    WRITE_ASM("call :%s\n", node->l_son->value.var_value);
    WRITE_ASM("push RCX\n\n");
    return 0;
}

)

DEF_LOG_OP_CMD(RETURN, 7, "return", "return",
{   
    handle_node(node->l_son, general_args);
    WRITE_ASM("pop RCX\n");

    WRITE_ASM("\npush VID\n");                    // right shift in RAM
    WRITE_ASM("dup\n");
    WRITE_ASM("dup\n");
    WRITE_ASM("push %d\n", SHIFT_IN_ARGS);
    WRITE_ASM("zdiv\n");
    WRITE_ASM("push 2\n");
    WRITE_ASM("sub\n");
    WRITE_ASM("push %d\n", SHIFT_IN_ARGS);
    WRITE_ASM("mul\n");
    WRITE_ASM("sub\n");
    WRITE_ASM("sub\n");
    WRITE_ASM("push %d\n", SHIFT_IN_ARGS);
    WRITE_ASM("add\n");
    WRITE_ASM("pop VID\n");


    WRITE_ASM("RET\n\n");


    return 0;
}

)

DEF_LOG_OP_CMD(INPUT, 8, "input", "input", 
{
    Node *tmp_node = node->l_son;

    while (tmp_node)
    {   
        WRITE_ASM("in\n")
        WRITE_ASM("pop [VID + %d]\n\n", find_index(general_args, tmp_node->value.var_value));
        // handle_node(tmp_node, general_args);
        tmp_node = tmp_node->l_son;
    }

    return 0;
}

)

DEF_LOG_OP_CMD(ELSE, 9, "else", "else", {})

DEF_LOG_OP_CMD(PRINT_STR, 10, "strprint", "strprint", 
{
    char str[MAX_LEN_VALUE] = {};

    strncpy(str, node->l_son->value.var_value, MAX_LEN_VALUE);

    int len = strlen(str);
    WRITE_ASM("push 0      ## %s\n", str);
    WRITE_ASM("push 10 \n");


    for (int i = len - 1; i >= 0; i--)
    {
        if (str[i] == 95)
        {
            WRITE_ASM ("push 32\n");
            continue;
        }
        WRITE_ASM("push %d\n", str[i]);
    }

    for (int i = 0; i < len + 2; i++)
    {
        WRITE_ASM("pop [%d]\n", i);
    }
    WRITE_ASM("show\n\n");

    return 0;
}

)
