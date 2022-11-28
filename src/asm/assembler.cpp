#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>
#include <debug.h> 
#include <asm.h>

// ./asm filename
// ./asm filename -o file.bin
// ./asm -o file.bin filename
// After releasing that you can learn getopt

int main(int argc, char* argv[])
{   
    open_ass_logs();
    
    const char* path_to_codes = name_of_input_file(argc, argv[1]); // ToDo: parse flags
    const char* path_to_executable_file_bin = "executable_file.bin"; // ToDo: argv

    FILE* file_text = fopen(path_to_codes, "r");
    CHECK_CONDITION(file_text != 0);

    FILE* executable_file     = fopen(path_to_executable_file_bin, "w");
    FILE* executable_file_bin = fopen(path_to_executable_file_bin, "wb");
    CHECK_CONDITION(executable_file != 0);
    CHECK_CONDITION(executable_file_bin != 0);

    ass_info info_of_codes = {};

    fill_info_of_exec_file(&info_of_codes, file_text, path_to_codes);  

    compile(executable_file, &info_of_codes);
   
    start_to_point_on_first_line(executable_file);
    fprintf(executable_file,"%s %d %zd\n", EXTENSION, VERSION, info_of_codes.num_commands);
    write_header_of_bin_file(executable_file_bin, EXTENSION, VERSION, info_of_codes.num_commands);
    
    compile(executable_file, &info_of_codes);
    
    DUMP_ASS (&info_of_codes, SIZE_OF_LABELS_ARR);

    fwrite(info_of_codes.arr_of_commands, sizeof(int), info_of_codes.num_commands, executable_file_bin);

    detor_info(&info_of_codes);
    fclose(file_text);
    fclose(executable_file);
    fclose(executable_file_bin);
    close_ass_logs();

    return 0;
}

void start_to_point_on_first_line(FILE *executable_file)
{
    fseek(executable_file, 0,SEEK_SET);
}
