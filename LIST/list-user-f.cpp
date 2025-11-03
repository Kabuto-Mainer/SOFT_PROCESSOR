#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "LIB/my-list.h"


// -------------------------------------------------------------------------------------------------------
int def_insertAfter(list_t* list,
                    lsi_t prev_index,
                    lsd_t value,
                    const char* name_func,
                    const char* name_file,
                    int number_line)
{
    assert(list);

#if VERIFIER == ON

    list_push_error_inf(list, name_file, number_line, name_func);

    char buffer[200] = {};
    int* ret_ver = &(list->list_inf.error_inf.current_error);

    list_verifier(list);

    if ((*ret_ver & L_NULL_DATA) != L_NULL_DATA)
    {
        // printf("PREF: %d  CAP: %zu\n", prev_index, list->list_inf.capacity);
        if ((size_t) prev_index >= list->list_inf.capacity)
        {
            *ret_ver |= L_INSERT_INDEX;
        }

        if (list->list_inf.current_size >= list->list_inf.capacity)
        {
            *ret_ver |= L_INSERT_SIZE;
        }

    }
        // printf("ERROR in insert: %d\n", list->list_inf.error_inf.current_error);
    if (*ret_ver != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from insertAfter before insert [" FORMAT_LSD_T "] after [" FORMAT_LSI_T "]\n", value, prev_index);
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }
    sprintf(buffer, "DUMP from insertAfter before insert [" FORMAT_LSD_T "] after [" FORMAT_LSI_T "]\n", value, prev_index);
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#else

    (void) name_func;
    (void) name_file;
    (void) number_line;

#endif // VERIFIER == ON

    int return_index = list_insert_after(list, prev_index, value);

#if VERIFIER == ON

    list_verifier(list);

    if (*ret_ver != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from insertAfter after insert [" FORMAT_LSD_T "] after [" FORMAT_LSI_T "]\n", value, prev_index);
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }

    sprintf(buffer, "DUMP from insertAfter after insert [" FORMAT_LSD_T "] after [" FORMAT_LSI_T "]\n", value, prev_index);
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#endif // VERIFIER == ON

    return return_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int def_deleteCurrent(list_t* list,
                      lsi_t del_index,
                      const char* name_func,
                      const char* name_file,
                      int number_line)
{
    assert(list);

#if VERIFIER == ON

    list_push_error_inf(list, name_file, number_line, name_func);

    char buffer[200] = {};
    int* ret_ver = &(list->list_inf.error_inf.current_error);

    list_verifier(list);
    if (*ret_ver != L_NOT_ERRORS && (*ret_ver & L_NULL_DATA) != L_NULL_DATA)
    {
        if ((size_t) del_index >= list->list_inf.current_size)
        {
            *ret_ver |= L_BAD_DEL;
        }

        if (list->list_inf.current_size == 0)
        {
            *ret_ver |= L_DELETE_SIZE;
        }
    }
    if (*ret_ver != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from deleteCurrent before delete [" FORMAT_LSI_T "]\n", del_index);
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }
    sprintf(buffer, "DUMP from deleteCurrent before delete [" FORMAT_LSI_T "]\n", del_index);
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#else
    (void) name_func;
    (void) name_file;
    (void) number_line;

#endif // VERIFIER == ON

    int return_index = list_delete_current(list, del_index);

#if VERIFIER == ON

    list_verifier(list);
    if (*ret_ver != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from deleteCurrent after delete [" FORMAT_LSI_T "]\n", del_index);
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }
    sprintf(buffer, "DUMP from deleteCurrent after delete [" FORMAT_LSI_T "] \n", del_index);
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#endif // VERIFIER == ON

    return return_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int def_deleteAfter(list_t* list,
                    lsi_t del_index,
                    const char* name_func,
                    const char* name_file,
                    int number_line)
{
    assert(list);

#if VERIFIER == ON

    list_push_error_inf(list, name_file, number_line, name_func);

    char buffer[200] = {};
    int* ret_ver = &(list->list_inf.error_inf.current_error);

    list_verifier(list);
    // printf("DEL INDEX: %d\n", del_index);
    if ((size_t) del_index >= list->list_inf.current_size)
    {
        *ret_ver |= L_BAD_DEL;
    }
    if (list->list_inf.current_size == 0)
    {
        *ret_ver |= L_DELETE_SIZE;
    }

    if (*ret_ver != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from deleteAfter before delete [" FORMAT_LSI_T "]\n", del_index);
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }

    sprintf(buffer, "DUMP from deleteAfter before delete [" FORMAT_LSI_T "]\n", del_index);
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#else
    (void) name_func;
    (void) name_file;
    (void) number_line;

#endif // VERIFIER == ON

    int return_index = list_delete_current(list, list->index_inf[del_index].next);

#if VERIFIER == ON

    list_verifier(list);
    if (*ret_ver != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from deleteAfter after delete [" FORMAT_LSI_T "]\n", del_index);
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }
    sprintf(buffer, "DUMP from deleteAfter after delete [" FORMAT_LSI_T "] \n", del_index);
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#endif // VERIFIER == ON

    return return_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int def_printList(list_t* list,
                  const char* name_func,
                  const char* name_file,
                  int number_line)
{
    assert(list);

#if VERIFIER == ON

    list_push_error_inf(list, name_file, number_line, name_func);

    char buffer[200] = "DUMP before printList";

    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#else
    (void) name_func;
    (void) name_file;
    (void) number_line;

#endif // VERIFIER == ON

    list_print(list);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int def_sortList(list_t* list,
                 const char* name_func,
                 const char* name_file,
                 int number_line)
{
    assert(list);

#if VERIFIER == ON

    list_push_error_inf(list, name_file, number_line, name_func);

    char buffer[200] = {};
    if (list_verifier(list) != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from sortList before sort\n");
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }

    sprintf(buffer, "DUMP from sortList before sort\n");
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#else

    (void) name_func;
    (void) name_file;
    (void) number_line;

#endif // VERIFIER == ON

    list_sort(list);

#if VERIFIER == ON

    if (list_verifier(list) != L_NOT_ERRORS)
    {
        sprintf(buffer, "ERROR from sortList after sort\n");
        list_dump_log(list, NAME_LOG_FILE, buffer);
        list_dump_html(list, NAME_DUMP_FILE, buffer);
        return -1;
    }

    sprintf(buffer, "DUMP from sortList after sort\n");
    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

#endif // VERIFIER == ON

    return 0;
}

