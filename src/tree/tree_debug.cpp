#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <math.h>
#include <tree_funcs.h>
#include <tree_debug.h>
#include <general_debug.h>
#include <diff_debug.h>

static FILE *TREE_LOGS = 0;
static FILE *HTM_LOGS  = 0;
static FILE *TREE_GRAPH_LOGS = 0;

static int get_op(Arith_Operation operation, char res[]);

static int get_log_op(Log_Oper log_operation, char res[]);

static int get_sep(Separators separator, char res[]);

int open_tree_logs()
{
    TREE_LOGS = fopen(TREE_LOGS_PATH, "w");

    if (NULL == TREE_LOGS)
    {
        printf("Cant open logs");
        return -1;
    }

    fprintf(TREE_LOGS,  "\\documentclass{article}\n"
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage{graphicx}\n"
                        "\\usepackage[14pt]{extsizes}\n"
                        "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n"
                        "\\begin{document}\n");
    fflush(TREE_LOGS);

    HTM_LOGS = fopen("graph_log.htm", "w");

    if (NULL == HTM_LOGS)
    {
        printf("Cant open graph logs for browser");
        return -1;
    }

    fprintf(HTM_LOGS, "<pre>\n\n");

    return 0;
}

int close_tree_logs()
{
    fclose(TREE_LOGS);
    fclose(HTM_LOGS);
    
    return 0;
}


int stringDump(const char *string, const char * name_of_var, const char * name_of_file, const char * name_of_func, int number_of_line)
{
    printf("\e[0;32m\n%s\e[0m at %s at %s(%d)\n",  name_of_var, name_of_func,
           name_of_file, number_of_line);

    printf("string = %s, string_length = %ld\n", string, strlen(string));

    return 0;
}


int tree_dump(Node *node, Mode_of_print mode,  const char* name_function, const char* name_file, const char* name_variable, int num_line)
{
    // TREE_LOGS = fopen(TREE_LOGS_PATH, "a+")

    fprintf(TREE_LOGS,  "Dump called from %s file, in %s func, in %d line, name of variable = %s\n\n",
                        name_file, name_function, num_line, name_variable);    
    fprintf(HTM_LOGS,   "<h2> Dump called from %s file, in %s func, in %d line, name of variable = %s\n\n",
                        name_file, name_function, num_line, name_variable);
    fprintf(TREE_LOGS, "\n$$\n");
    tree_print(node, mode);
    fprintf(TREE_LOGS, "\n$$\n");
    fflush(TREE_LOGS);

    return 0;
}

int open_log_pdf()
{   
    TREE_LOGS = fopen(TREE_LOGS_PATH, "a+");
    fprintf(TREE_LOGS,  "\\end{document}");
    fflush(TREE_LOGS);
    
    char command[512] ={};
    sprintf(command, "echo pdflatex %s > nul 2>&1", TREE_LOGS_PATH);
    system(command);
    fclose(TREE_LOGS);

    system("xdg-open tree_logs.pdf");
    TREE_LOGS = fopen(TREE_LOGS_PATH, "a+");
    
    return 0;
}

int tree_print(const Node *node, const Mode_of_print mode)
{
    if (!node) 
    {
        return 0;
    }
    Arith_Operation op = node->value.op_value;
   

    if (node->parent && node->priority && node->parent->priority > node->priority)
    {
        fprintf(TREE_LOGS, "(");
    }

    PRINT_DIFF_IN_LOG_IF(mode == PREORDER, node);

    if(node->l_son)
        {   
            tree_print(node->l_son, mode);
        }

    PRINT_DIFF_IN_LOG_IF(mode == INORDER, node);

    if (node->r_son)
    {
        tree_print(node->r_son, mode);
    }
    
    PRINT_DIFF_IN_LOG_IF(mode == POSTORDER, node);
    
    if (node->parent && node->priority && node->parent->priority > node->priority)
    {
        fprintf(TREE_LOGS, ")");
    }

    
    fflush(TREE_LOGS);
    return 0;
}

int tree_print_graph(const Node *node);

static int num_of_node = 1;
static int num_of_pic = 1;

int tree_graph_dump(Node *node, Mode_of_print mode,  const char* name_function, const char* name_file, const char* name_variable, int num_line)
{   
    TREE_GRAPH_LOGS = fopen("graph_log.html", "w");

    if (NULL == TREE_GRAPH_LOGS)
    {
        printf("Cant open graph logs");
        return -1;
    }

    fputs("digraph lala{\n", TREE_GRAPH_LOGS);
    fputs("rankdir = HR;\n", TREE_GRAPH_LOGS);
    fputs("bgcolor = grey;\n", TREE_GRAPH_LOGS);

    fputs("graph [splines=polyline];\n", TREE_GRAPH_LOGS);
    fputs("node [shape = \"plaintext\", style = \"solid\"];\n", TREE_GRAPH_LOGS);
    
    tree_print_graph(node);
    
    fputs("\n}", TREE_GRAPH_LOGS);

    fprintf(HTM_LOGS, "DUMP #%d \n", num_of_pic);
    fprintf(HTM_LOGS, "<img src = graph_dumps/dump_%d.jpeg>\n", num_of_pic);

    char command[128] = {};   
    sprintf(command, "dot -Tjpeg graph_log.html > graph_dumps/dump_%d.jpeg", num_of_pic++);
    
    fclose(TREE_GRAPH_LOGS);
    system(command);
    return 0;
}

int tree_show_graph()
{
    char command[256] = {};
    int cur_num_pic = num_of_pic - 1;

    sprintf(command, "xdg-open graph_dumps/dump_%d.jpeg", cur_num_pic);

    system(command);

    return 0;
}



int tree_print_graph(const Node *node)
{
    if (!node)  return 0;

    int cur_node = num_of_node;

    fprintf(TREE_GRAPH_LOGS, "node%d [\n"
                        "label=<\n"
                        
                        "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n"
                        
                        "    <tr><td bgcolor=\"#ad8abf\"><font color=\"#061f2b\">parent = %p</font></td></tr>\n",
                        num_of_node, node->parent);
    switch(node->type)
    {
        case VAR:
        {
            fprintf(TREE_GRAPH_LOGS, "    <tr><td bgcolor=\"lightblue\"><font color=\"#0000ff\"> %s </font></td></tr>\n",
             node->value.var_value);
            break;
        }
        case NUM:
        {
            fprintf(TREE_GRAPH_LOGS, "    <tr><td bgcolor=\"lightblue\"><font color=\"#0000ff\"> %g </font></td></tr>\n",
            node->value.dbl_value);
            break;
        }
        case ARITHM_OP:
        {   
            char res[MAX_LEN_VALUE] = {};
            get_op(node->value.op_value, res);
            fprintf(TREE_GRAPH_LOGS, "    <tr><td bgcolor=\"lightblue\"><font color=\"#0000ff\"> %s </font></td></tr>\n", 
                                    res);
            break;
        }
        case SEP:
        {   
            char res[MAX_LEN_VALUE] = {};
            get_sep(node->value.sep, res);
        }
        case LOG:
        {
            char res[MAX_LEN_VALUE] = {};
            get_log_op(node->value.log_op, res);
            fprintf(TREE_GRAPH_LOGS, "    <tr><td bgcolor=\"lightblue\"><font color=\"#0000ff\"> %s </font></td></tr>\n", 
                                    res);
            break;
        }
        default: SOFT_ASS_NO_RET(1);
    }
    fprintf(TREE_GRAPH_LOGS, "    <tr><td bgcolor=\"#c0fae8 \"><font color=\"black\">priority = %d</font></td></tr>\n", node->priority);

    fprintf(TREE_GRAPH_LOGS, "    <tr><td bgcolor=\"lightgreen\"><font color=\"black\">cur_address = %p</font></td></tr>\n"
                        
                        "    <tr>\n"
                        "    <td>\n"
                        
                        "        <table border=\"0\" cellborder=\"1\">\n"
                        
                        "        <tr>\n"
                        
                        "            <td bgcolor=\"#70de9f\" port = \"L%d\"> Left = %p</td> \n"
                        
                        "            <td bgcolor = \"#c8A2c8\" port = \"R%d\"> Right =  %p </td>\n"
                        
                        "        </tr> \n"
                        
                        "        </table> \n"
                        
                        "    </td>\n"
                        
                        "    </tr>\n" 
                        
                        "</table>>\n"
                        "]\n\n", node, num_of_node, node->l_son, num_of_node, node->r_son);
    num_of_node++;   

    if(node->l_son)
    {   
        fprintf(TREE_GRAPH_LOGS, "node%d: L%d -> node%d; \n", cur_node, cur_node, num_of_node);
        tree_print_graph(node->l_son);
    }

    if (node->r_son)
    {   
        fprintf(TREE_GRAPH_LOGS, "node%d: R%d -> node%d; \n", cur_node, cur_node, num_of_node);
        tree_print_graph(node->r_son);
    }

    return 0;
}

int tree_error_decoder(int code)
{
    TREE_PRINT_ERROR(code, TREE_ERROR_REWRITE_NODE);


    return 0;
}

int print_in_logs(const char *str,...)
{   
    if (TREE_LOGS != NULL && str != NULL)
    {
        va_list args;
        
        va_start(args, str);
        vfprintf(TREE_LOGS, str, args);
        va_end(args);
        
        fflush(TREE_LOGS);
    }

    return 0;
}

#define DEF_SEP(sep, code, naive_name, custom_name)         \
    case sep:                                               \
    {                                                       \
        strncpy(res, naive_name, MAX_LEN_VALUE);            \
        break;                                              \
    }

static int get_sep(Separators separator, char res[])
{
    switch (separator)
    {
        #include <separators.h>
    default:
        strncpy(res, "ERROR", MAX_LEN_VALUE);
    }

    return 0;
}

#undef DEF_SEP

#define DEF_LOG_OP_CMD(op_name, code, naive_name, custom_name)       \
        case op_name:                                           \
        {                                                       \
            strncpy(res, naive_name, MAX_LEN_VALUE);            \
            break;                                              \
        }

static int get_log_op(Log_Oper log_operation, char res[])
{
    switch (log_operation)
    {
    #include <log_operation.h>

    default:
    {
        SOFT_ASS(1);
    }
    }

    return 0;
}

#undef DEF_LOG_OP_CMD

#define DEF_ARITH_CMD(op_name, code, naive_name, custom_name)       \
        case op_name:                                           \
        {                                                       \
            strncpy(res, naive_name, MAX_LEN_VALUE);            \
            break;                                              \
        }

static int get_op(Arith_Operation operation, char res[])
{
    switch(operation)
    {   
        #include <operation.h>

        default:
        {
            strncpy(res, "ERROR", MAX_LEN_VALUE);
            break;
        }
    }

    return 0;
}

#undef DEF_ARITH_CMD