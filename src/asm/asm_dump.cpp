#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "../include/asm.h"
#include "../include/stack_objects.h"
#include "../include/debug.h"
#include "../include/cpu.h"

static FILE* ASS_LOGS = 0; // ToDo: USE_CAPS_WITH_GLOBAL_VARIABLES


void open_ass_logs() // ToDo: filename
{
    ASS_LOGS = fopen("ass_logs.txt", "w"); // ToDo: assert is bad!
    assert(ASS_LOGS != NULL);
}

void close_ass_logs(void)
{
    fclose(ASS_LOGS);
}

void dump_ass (ass_info *info_of_codes, int size, const char* file_name, const char* func_name, int line)
{  // ToDo: check is file opened
    fprintf(ASS_LOGS, "DUMP CALLED FROM %s file, %s func, %d line \n\n", file_name, func_name, line);

    fprintf(ASS_LOGS, "buff_size = %zd \n", info_of_codes->num_of_sym);           
    fprintf(ASS_LOGS, "num of str = %zd \n", info_of_codes->num_of_lines);        
    fprintf(ASS_LOGS, "num_of_commands = %zd\n", info_of_codes->num_commands);

    fputs("\n\nDUMP BUFFER:\n\n", ASS_LOGS);
    for (size_t i = 0; i < info_of_codes->num_of_sym; i++)
    {           
        if (info_of_codes->buffer[i] == '\0')
        {
            fprintf(ASS_LOGS, "\n");
            continue;
        }
        fprintf(ASS_LOGS, "%c", info_of_codes->buffer[i]);                         
    }

    fputs("\n\nDUMP LINES:\n\n", ASS_LOGS);
    for (size_t i = 0; i < info_of_codes->num_of_lines; i++)         
        fprintf(ASS_LOGS,"%s\n", info_of_codes->text[i]); 
                                        
    fputs("\n\nDUMP OF LABELS:\n\n", ASS_LOGS);
    for(int i = 0; i < size; i++)
    {
        if (info_of_codes->labels[i].address != -1)
        {
            fprintf(ASS_LOGS, "* %s = %2d   \n", info_of_codes->labels[i].label_name, info_of_codes->labels[i].address);
        }
        else if (info_of_codes->labels[i].address == -1)
        {
            fprintf(ASS_LOGS, " %s = %2d (POISON)\n", info_of_codes->labels[i].label_name, info_of_codes->labels[i].address);
        }
    }
}

