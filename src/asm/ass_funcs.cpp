#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>
#include <debug.h>
#include <asm.h>

int compile(FILE *executable_file, ass_info *info_of_codes)
{
    size_t line = 0;
    int shift = 0;
    int label_ptr = 0;
    char label_name[128] = {};
    int ip = 0;
    char cmd[256] = {};
    CHECK_CONDITION(cmd != 0);

    #define DEF_CMD(name, num, arg, end_sym, getarg_code,...)                                                   \
            else if (strcmp(cmd, #name) == 0)                                                                   \
            {                                                                                                   \
                write_to_files(executable_file, info_of_codes->arr_of_commands, num, &ip, end_sym);             \
                                                                                                                \
                if(arg == 1)                                                                                    \
                    get_args(info_of_codes, args_begin, info_of_codes->arr_of_commands, &ip, executable_file);  \
            }

    while (line < info_of_codes->num_of_lines)
    {   
        sscanf(info_of_codes->text[line], "%s%n", cmd, &shift);
        
        char* args_begin = info_of_codes->text[line] + shift + 1;

        if (strchr(cmd, ':') != 0)
        {

            strncpy(label_name, find_label_name(cmd), 128);

            if (strcmp(label_name, "ERROR") == 0)
            {
                puts("Wrong label\n");
            }           
            else
            {   
                *(strchr(label_name, ':')) = '\0';
                strncpy(info_of_codes->labels[label_ptr].label_name, label_name, 128);
                info_of_codes->labels[label_ptr].address = ip;
            }
            label_ptr++;
        }

        #include "../include/cmd.h"

        else if (cmd[0] == -1); 

        else
        {
            printf("wrong command on %d position\n", ip);
        }

        cmd[0] = -1;
        line++;   
    }
    return 0;
}
#undef DEF_CMD

#define DEF_CMD(name, num, arg, end_sym, getarg_code,...) \
    case num:                                             \
    {                                                     \
        getarg_code;                                      \
        break;                                            \
    }


var_ass get_args(ass_info *info_of_codes, char *text, int *arr_of_commands, int *ip, FILE* executable_file)
{   
    pair args = {};
    args = get_arg(text);

    switch (arr_of_commands[*ip-1])
    {    
        #include "../include/cmd.h"
    }
    return 0;
}

#undef DEF_CMD

size_t know_size_for_buff(FILE* text, const char * name_of_file)
{
    CHECK_CONDITION(text != 0);
    CHECK_CONDITION(name_of_file != 0);

    struct stat data = {};
    stat(name_of_file, &data);

    if (((data.st_mode & S_IFMT) - S_IFDIR) == 0)
        {
            printf("Trying to find size of dir");
            abort();
        }

    return data.st_size;
}

int fill_info_of_exec_file(ass_info *info_of_codes, FILE* file_ptr, const char* file_path)
{   
    CHECK_CONDITION(info_of_codes != 0);   
    CHECK_CONDITION(file_ptr != 0);
    CHECK_CONDITION(file_path != 0);

    info_of_codes->num_commands = 0;

    info_of_codes->num_of_sym = know_size_for_buff(file_ptr, file_path) + 1;
    CHECK_CONDITION(info_of_codes->num_of_sym != 0);

    info_of_codes->buffer = (char*) calloc(info_of_codes->num_of_sym, sizeof(char));
    CHECK_CONDITION(info_of_codes->buffer != 0); 
    
    int test_fread = 0;
    test_fread = fread(info_of_codes->buffer, info_of_codes->num_of_sym - 1, sizeof(int), file_ptr);
    info_of_codes->buffer[info_of_codes->num_of_sym - 1] = '\0';
    CHECK_CONDITION(test_fread !=0);

    info_of_codes->num_of_lines = make_ptr_arr_of_lines(info_of_codes);

    fill_labels_bad_values(info_of_codes->labels, SIZE_OF_LABELS_ARR);

    info_of_codes->arr_of_commands = (int*) calloc(info_of_codes->num_of_sym + 5*sizeof(int), sizeof(char));
    CHECK_CONDITION(info_of_codes->arr_of_commands != 0);

    return 0;
}

int make_ptr_arr_of_lines(ass_info *info_of_codes)
{
    size_t str_counter = 1; 
    size_t curr_sym = 0;
    int flag = 1;
    
    for (; curr_sym < info_of_codes->num_of_sym; curr_sym++)
    {    
        if (flag == 1 && isalnum(info_of_codes->buffer[curr_sym]))
        {
            info_of_codes->num_commands++;
            flag = 0;
        }
        
        else if((curr_sym > 0) && ((isdigit(info_of_codes->buffer[curr_sym])) && info_of_codes->buffer[curr_sym] == ':')) 
        {
            info_of_codes->num_commands--;
        }
        
        else if (!(isalnum(info_of_codes->buffer[curr_sym])))            
            flag = 1;

        if ((info_of_codes->buffer[curr_sym] == '\n'))
        {
            str_counter++;

            info_of_codes->buffer[curr_sym] = '\0';
        } 
        
        else if (info_of_codes->buffer[curr_sym] == '#' && info_of_codes->buffer[curr_sym+1] == '#')
        {
            info_of_codes->buffer[curr_sym] = '\0';

            while (info_of_codes->buffer[curr_sym++] != '\n')
            {}
            curr_sym -= 2;
        }
    }
    
    info_of_codes->text = (char**) calloc(str_counter, sizeof(char*));
    CHECK_CONDITION(info_of_codes->text != 0);

    size_t line = 1;
    curr_sym = 0;

    info_of_codes->text[0] = info_of_codes->buffer + curr_sym;

    for(; (curr_sym < info_of_codes->num_of_sym - 1); curr_sym++)
    {
      if ((curr_sym > 0) && (info_of_codes->buffer[curr_sym] =='\0') && (info_of_codes->buffer[curr_sym+1] !='#'))
        {
            info_of_codes->text[line] = info_of_codes->buffer + curr_sym + 1;
            line++; 
        }
    }

    return str_counter;
}

void detor_info(ass_info *info_of_codes) // ToDo: maybe poison?
{   
    free(info_of_codes->buffer);
    free(info_of_codes->arr_of_commands);
    free(info_of_codes->text);
}

void write_header_of_bin_file(FILE *exec_bin_file, const char* extension, int version, int num_of_commands)
{
    int arr[3] = {};
    arr[0] += extension[0];
    arr[0] += extension[1] << 8;
    arr[0] += extension[2] << 16;

    arr[1] = version;
    arr[2] = num_of_commands;
    fwrite(arr, sizeof(int), 3, exec_bin_file);
}

void fill_labels_bad_values(Labels_t *labels, int size)
{
    for (int i = 0; i < size; i++)
    {
        labels[i].address = POISON;
    }
}

void write_to_files(FILE* executable_file, int *arr_of_cmd, int target_num, int *ip, char end_char)
{
    fprintf(executable_file, "%d%c", target_num, end_char);
    arr_of_cmd[(*ip)++] = target_num;
}

#define WRITE_REG()     if (strcmp("RAX", tmp_str) == 0)            \
                        {                                           \
                            tmp_pair.type |= ARG_REG;               \
                            tmp_pair.value = RAX;                   \
                        }                                           \
                        else if (strcmp("RBX", tmp_str) == 0)       \
                        {                                           \
                            tmp_pair.type |= ARG_REG;               \
                            tmp_pair.value = RBX;                   \
                        }                                           \
                        else if (strcmp("RCX", tmp_str) == 0)       \
                        {                                           \
                            tmp_pair.type |= ARG_REG;               \
                            tmp_pair.value = RCX;                   \
                        }                                           \
                        else if (strcmp("RDX", tmp_str) == 0)       \
                        {                                           \
                            tmp_pair.type |= ARG_REG;               \
                            tmp_pair.value = RDX;                   \
                        }

pair get_arg(char *str)
{   
    int shift = 0;
    char tmp_str[256] ={};
    int tmp_value = 0;
    pair tmp_pair = {};
    
    sscanf(str, " %s%n", tmp_str, &shift);
    
    if (isalpha(str[0]))
    {   
        
        WRITE_REG();
    }

    if (sscanf(str, "%d", &tmp_pair.value))
    {
        tmp_pair.type |= ARG_IMMED;
    }   

    if ((strchr(str, '[') != NULL) && (strchr(str, ']') != NULL) && (strchr(str, '[') < strchr(str, ']'))) 
    {
        tmp_pair.type |= ARG_RAM;
        if (sscanf(strchr(str, '[') + 1, "%d", &tmp_value) == 1)
        {
            tmp_pair.value = tmp_value;
        }

        else if (sscanf(strchr(str, '[') + 1, "%s", tmp_str))
        {
            tmp_str[3] = '\0';

            WRITE_REG();   
        }
    }

    return tmp_pair;
}

#undef WRITE_REG

const char *find_label_name(char *str)
{
    int shift = 0;
    char *address_of_label_sym = strchr(str, ':');
    char *right_ptr = str;

    while (isspace(str[shift]) == 1)
        shift++;

    right_ptr += shift;

    if (right_ptr < address_of_label_sym && ((strchr(right_ptr, ' ') == 0) || (address_of_label_sym < strchr(right_ptr, ' '))) )
        {
            return right_ptr;
        }
    
    else
    {
        return "ERROR";
    }
    
}

int find_address(char *label_name, ass_info *info_of_codes)
{
    int address = 0;
    
    while(address < SIZE_OF_LABELS_ARR)
    {  
        if (strcmp(label_name, info_of_codes->labels[address].label_name) == 0)
            break;
        address++;
    }
    
    if (address == SIZE_OF_LABELS_ARR)
    {
        return -1;
    }
    else
        return address;
}

const char *name_of_input_file(int num_of_str, const char* str)
{
    if (num_of_str > 1)
        return str;
    else 
        return "codes.txt";
}