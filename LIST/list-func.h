#ifndef LIST_FUNC_H
#define LIST_FUNC_H

#include "list-type.h"

// -------------------------------------------------------------------------------------------------------
list_error_t list_ctr(list_t* list,
                      list_inf_t list_inf);
// -------------------------------------------------------------------------------------------------------
list_error_t list_dtr(list_t* list);
// -------------------------------------------------------------------------------------------------------
int clean_log_file(void);
// -------------------------------------------------------------------------------------------------------
int fulling_list_inf(list_inf_t* input_list_inf,
                     list_inf_t* list_inf);
// -------------------------------------------------------------------------------------------------------
list_error_t list_insert_after(list_t* list,
                               lsi_t prev_index,
                               lsd_t value);
// -------------------------------------------------------------------------------------------------------
list_error_t list_append(list_t* list,
                         lsd_t value);
// -------------------------------------------------------------------------------------------------------
list_error_t list_delete_current(list_t* list,
                                 lsi_t del_index);
// -------------------------------------------------------------------------------------------------------
list_error_t list_delete_after(list_t* list,
                               lsi_t before_del_index);
// -------------------------------------------------------------------------------------------------------
int list_verifier(list_t* list,
                  const char* name_file,
                  const int number_line,
                  const char* name_func);
// -------------------------------------------------------------------------------------------------------
list_error_t list_check_del(list_t* list,
                            lsi_t index);
// -------------------------------------------------------------------------------------------------------
list_error_t list_check_insert(list_t* list,
                               lsi_t index,
                               lsd_t value);
// -------------------------------------------------------------------------------------------------------
list_error_t list_dump(list_t* list,
                       const char* name_file,
                       const char* dump_reason);
// -------------------------------------------------------------------------------------------------------
list_error_t list_print(list_t* list);
// -------------------------------------------------------------------------------------------------------
int create_head_html_file(void);
// -------------------------------------------------------------------------------------------------------
int create_head_html_block(FILE* file,
                           list_dump_t type_dump);
// -------------------------------------------------------------------------------------------------------
int list_dump_html(list_t* list,
                   const char* name_html_file,
                   const char* dump_reason);
// -------------------------------------------------------------------------------------------------------
list_error_t create_graph(list_t* list);
// -------------------------------------------------------------------------------------------------------
#define COMMON_VERIFIER(list, name_func, return_value, dump_reason) \
    if (list_verifier(list, __FILE__, __LINE__, name_func) != L_NOT_ERRORS) \
    { \
        list_dump(list, NAME_LOG_FILE, dump_reason); \
        list_dump_html(list, NAME_DUMP_FILE, dump_reason); \
        return return_value; \
    } \
    list_dump(list, NAME_LOG_FILE, dump_reason); \
    list_dump_html(list, NAME_DUMP_FILE, dump_reason);



#endif // LIST_FUNC_H
