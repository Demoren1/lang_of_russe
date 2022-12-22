#ifndef DIFF_DEBUG_GUARD
#define DIFF_DEBUG_GUARD

#define PRINT_DIFF_IN_LOG_IF(condition, node)       if (condition)                                          \
                                                        {                                                       \
                                                            if (node->type == ARITHM_OP)                               \
                                                            {                                                   \
                                                                char res[MAX_LEN_VALUE];                        \
                                                                get_op(node->value.op_value, res);              \
                                                                fprintf(TREE_LOGS, " %s ", res);                  \
                                                            }                                                   \
                                                            else if (node->type == LOG)                         \
                                                            {                                                   \
                                                                char res[MAX_LEN_VALUE];                         \
                                                                get_log_op(node->value.log_op, res);           \
                                                                fprintf(TREE_LOGS, " %s ", res);                 \
                                                            }                                                   \
                                                            else if(node->type == VAR)                          \
                                                            {                                                   \
                                                                fprintf(TREE_LOGS, "%s", node->value.var_value);      \
                                                            }                                                   \
                                                            else if(node->type == NUM)                          \
                                                            {                                                   \
                                                                fprintf(TREE_LOGS, "%g", node->value.dbl_value);      \
                                                            }                                                   \
                                                        }


#endif                                                        