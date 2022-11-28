#define var double
#define push(num) stack_push(stack, num);
#define pop(num) stack_pop(stack, &num)
#define cell_value (ssize_t)cpu->num_buffer[ip]
#define JMP_CONDTION(condition) if (condition)                      \
                                    ip = cell_value;                \
                                else                                \
                                    ip++;                           
#define JMP_GETARG  {                                                                                                                                   \
                        int label_ptr = 0;                                                                                                              \
                        char label_name[128] = {};                                                                                                      \
                        sscanf(text, " :%s", label_name);                                                                                               \
                        label_ptr = find_address(label_name, info_of_codes);                                                                            \
                        if (info_of_codes->labels[label_ptr].address != -1)                                                                             \
                            write_to_files(executable_file, info_of_codes->arr_of_commands, info_of_codes->labels[label_ptr].address, ip, '\n');        \
                        break;                                                                                                                          \
                    }                                                                                                                               

DEF_CMD(HLT, 0, 0, '\n',
{}, 
{
    break;
}
)

DEF_CMD (PUSH, 1, 1, ' ',
{
    arr_of_commands[*ip-1] |= args.type;
            fseek(executable_file, -2, SEEK_CUR);
            fprintf(executable_file, "%d%c", arr_of_commands[*ip-1],' ');
                        
            arr_of_commands[*ip] = args.value;
            fprintf(executable_file, "%d %c", arr_of_commands[(*ip)++],'\n');
            
            break;
},
{
    if(full_cmd & ARG_IMMED) 
        num += cell_value;            

    if(full_cmd & ARG_REG)
        num += cpu->registers[cell_value] / (var)ACCURACY;

    if(full_cmd & ARG_RAM)   
    {
        num += cpu->RAM[cell_value] / (var)ACCURACY;
    }

    if((full_cmd & ARG_RAM) && (full_cmd & ARG_REG))
    {
        num += cpu->RAM[cpu->registers[cell_value]];
    }

    ip++;
    push(num * ACCURACY);
}
)

DEF_CMD(POP, 2, 1, ' ',
{arr_of_commands[*ip-1] |= args.type;
            fseek(executable_file, -2, SEEK_CUR);
            fprintf(executable_file, "%d%c", arr_of_commands[*ip-1],' ');
                        
            arr_of_commands[*ip] = args.value;
            fprintf(executable_file, "%d %c", arr_of_commands[(*ip)++],'\n');
            
            break;
},
{
    pop(num);

    if(full_cmd & ARG_REG)   
        cpu->registers[cell_value] = (int) num;

    if(full_cmd & ARG_RAM)   
        cpu->RAM[cell_value] = (int) num;
            
    if((full_cmd & ARG_RAM) && (full_cmd & ARG_REG))
    {
        cpu->RAM[cpu->registers[cell_value]] = (int)num;
    }

    ip++;
}
)

DEF_CMD(ADD, 3, 0, '\n',
{},
{   
    pop(num1);           
    pop(num2);

    push(num1 + num2);
}
)

DEF_CMD(SUB, 4, 0, '\n',
{},
{   
    pop(num1);           
    pop(num2);

    push(num2 - num1);
}
)

DEF_CMD(MUL, 5, 0, '\n',
{},
{    
    pop(num1);           
    pop(num2);

    push(num1 * num2 / ACCURACY)
}
)


DEF_CMD(DIV, 6, 0, '\n',
{},
{
    pop(num1);           
    pop(num2);
    
    num = (num2 * ACCURACY) / num1;

    push(num);
}
)

DEF_CMD(IN, 7, 0, '\n',
{},
{
    scanf("%lf", &num);

    push(num*1000);
}
)

DEF_CMD(OUT, 31, 0, '\n',
{},
{
    pop(num);
    
    fprintf(file_result, "%d", (int)num);
    fprintf(stdout, "%g \n", (num / ACCURACY));
}
)

DEF_CMD(DUMP, -1, 0, '\n',
{},
{
    DUMP_CPU(*cpu, ip, stack);
}
)

DEF_CMD(DUP, 8, 0, '\n',
{},
{
    pop(num);
    push(num);
    push(num);
}
)

DEF_CMD(JMP, 9, 1, ' ',
{JMP_GETARG},
{
    ip = cell_value;            
}
)

DEF_CMD(JA, 10, 1, ' ',
{JMP_GETARG},
{
    pop(num1);
    pop(num2);
    JMP_CONDTION(num2 > num1);
}
)

DEF_CMD(JAE, 11, 1, ' ', 
{JMP_GETARG},
{               
    pop(num1);
    pop(num2);
    
    JMP_CONDTION(num2 >= num1);
}
)

DEF_CMD(JEE, 12, 1, ' ',
{JMP_GETARG},
{
    pop(num1);
    pop(num2);
    JMP_CONDTION(num1 == num2)
}
)

DEF_CMD(JBE, 13, 1, ' ',
{JMP_GETARG},
{
    pop(num1);
    pop(num2);
    JMP_CONDTION(num2 <= num1)
}
)

DEF_CMD(JB, 14, 1, ' ',
{JMP_GETARG},
{
    pop(num1);
    pop(num2);
    JMP_CONDTION(num2 < num1)
}
)

DEF_CMD(JNE, 15, 1, ' ',
{JMP_GETARG},
{
    pop(num1);
    pop(num2);
    JMP_CONDTION(num1 != num2)
}
)

DEF_CMD(CALL, 16, 1, ' ',
{JMP_GETARG},
{
    stack_push(&(cpu->func_stack), (elem)ip);

    ip = cell_value;
}
)

DEF_CMD(RET, 17, 1, '\n',
{JMP_GETARG},
{
    elem func_addres = 0;
    stack_pop(&(cpu->func_stack), &func_addres);

    ip = (ssize_t) func_addres;
    ip++;
}
)

DEF_CMD(SHOW, 18, 0, '\n',
{},
{
    int counter = 0;
    while ((cpu->RAM[counter] != '\0') && (counter < SIZE_OF_RAM))
    {
        printf("%c", cpu->RAM[counter] / ACCURACY);
        counter++;
    }
}
)

DEF_CMD(OUTF, 19, 0, '\n',
{},
{   
    pop(num);
    
    fprintf(file_result, "%d", (int)num);
    fprintf(stdout, "%g \n", num);
}
)

DEF_CMD(SQRT, 20, 0, '\n',
{},
{
    var sqrtt = 0;
    var pitch = 1 / ((var)ACCURACY);
    pop(num);

    if (num < 0)
    {
        printf("Can't find sqrt of negative value");
        push(num);
    }
    else
    {
        while(sqrtt * sqrtt < num)
        {   
            sqrtt += pitch;
        }
        push(sqrtt * sqrt((var)ACCURACY));
    }
}
)

DEF_CMD(DBG, 21, 1, '\n',
{},
{
    printf("ip = %zd\n", ip);
}
)

DEF_CMD(SIN, 22, 0, '\n',
{},
{
    pop(num);
    num = num / ACCURACY;
    num = sin(num);
    push(num * ACCURACY);
})

DEF_CMD(COS, 23, 0, '\n',
{},
{
    pop(num);
    num = num / ACCURACY;
    num = cos(num);
    push(num * ACCURACY);
})

DEF_CMD(INCR_R, 24, 1, '\n',
{           
    arr_of_commands[*ip] = args.value;
    fprintf(executable_file, "%d %c", arr_of_commands[(*ip)++],'\n');
},
{
    int tmp = cpu->registers[cell_value];
    tmp += ACCURACY;
    cpu->registers[cell_value] = tmp;
    ip++;
})






























































