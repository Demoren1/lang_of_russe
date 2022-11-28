#ifndef ASM_GUARD
#define ASM_GUARD
#define VERSION 1
extern const char *EXTENSION;

#define POISON -1

const int ACCURACY = 1000;

const int SIZE_OF_LABELS_ARR = 100;

typedef struct Labels_
{
    char label_name[128];
    int address;
} Labels_t;

struct ass_info
{
    char** text;
    char* buffer;
    int *arr_of_commands;
    Labels_t labels[SIZE_OF_LABELS_ARR];
    size_t num_of_lines;
    size_t num_commands;
    size_t num_of_sym;
};

struct pair
{
    int type; 
    int value;
};

typedef int var_ass;

pair get_arg(char *str);

const char *name_of_input_file(int num_of_str, const char* str);

size_t know_size_for_buff(FILE* text, const char * name_of_file);

int fill_info_of_exec_file(ass_info *info_of_codes, FILE* file_ptr, const char* file_path);

const char *find_label_name(char *str);

int find_address(char *label_name, ass_info *info_of_codes);

int make_ptr_arr_of_lines(ass_info *info_of_codes);

void detor_info(ass_info *info_of_codes);

int compile(FILE *executable_file, ass_info *info_of_codes);

void write_header_of_bin_file(FILE *exec_bin_file, const char* extension, int version, int num_of_commands);

void fill_labels_bad_values(Labels_t *labels, int SIZE_OF_LABELS_ARR);

void dump_ass (ass_info *info_of_codes, int size, const char* file_name, const char* func_name, int line);

FILE* open_with_no_buff(const char* name_file, const char* regime);

void write_to_files(FILE* executable_file, int *arr_of_cmd, int target_num, int *cmd_counter, const char end_char);

var_ass get_args(ass_info *info_of_codes, char *text, int *arr_of_commands, int *ip, FILE* executable_file);

void start_to_point_on_first_line(FILE *executable_file);

void open_ass_logs();

void close_ass_logs();

const int MASK_CMD = 0x3F;

#define DEF_CMD(name, num, arg, end_sym, getarg_code,...)    \
    name ## _CMD = num,                

enum CPU_codes
{   
    #include "../include/cmd.h"
    MAX_CMD
};
#undef DEF_CMD

enum Registers
{   
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4
};

enum Masks
{   
    ARG_IMMED = 0x40, 
    ARG_REG   = 0x80,
    ARG_RAM   = 0x100
};

#endif
