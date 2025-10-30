#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "list-type.h"
#include "list-config.h"
#include "list-const.h"
#include "list-func.h"

#include "../COMMON/support.h"
#include "../COMMON/color.h"

static int AMOUNT_IMAGES = 0;


// Управление объектом
// -------------------------------------------------------------------------------------------------------
list_error_t list_ctr(list_t* list,
                      list_inf_t list_inf)
{
    assert(list);

    lsd_t* buf_data = (lsd_t*) calloc(START_SIZE_LIST, sizeof(lsd_t));
    if (buf_data == NULL)
    {
        return L_CALLOC_NULL;
    }

    index_t* buf_index = (index_t*) calloc(START_SIZE_LIST, sizeof(index_t));
    if(buf_index == NULL)
    {
        return L_CALLOC_NULL;
    }

    for (int i = 0; i < START_SIZE_LIST - 1; i++)
    {
        buf_index[i].next = i + 1;
        buf_index[i].prev = i - 1;
    }
    buf_index[0].next = 0;
    buf_index[0].prev = 1;
    buf_index[1].prev = START_SIZE_LIST - 1;

    buf_index[START_SIZE_LIST - 1].next = 1;
    buf_index[START_SIZE_LIST - 1].prev = START_SIZE_LIST - 2;

    for (int i = 0; i < START_SIZE_LIST; i++)
    {
        buf_data[i] = L_FREE_NUM;
    }
    // buf_data[0] = 0;

    list->free = 1;

    list->data = buf_data;
    list->index_inf = buf_index;

    fulling_list_inf(&list_inf, &(list->list_inf));
    clean_log_file();
    create_head_html_file();

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int fulling_list_inf(list_inf_t* input_list_inf,
                     list_inf_t* list_inf)
{
    assert(input_list_inf);
    assert(list_inf);

    list_inf->create_inf.name_file = input_list_inf->create_inf.name_file;
    list_inf->create_inf.name_list = input_list_inf->create_inf.name_list;
    list_inf->create_inf.number_line = input_list_inf->create_inf.number_line;
    list_inf->create_inf.start_size = START_SIZE_LIST;

    list_inf->error_inf.current_error = L_NOT_ERRORS;
    list_inf->error_inf.name_call_file = NULL;
    list_inf->error_inf.name_call_func = NULL;
    list_inf->error_inf.number_call_line = -1;
    list_inf->current_size = 0;
    list_inf->capacity = START_SIZE_LIST;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    list_inf->create_inf.time_start = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000) % TIME_DIV;


    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_dtr(list_t* list)
{
    assert(list);

    free(list->data);
    free(list->index_inf);

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------



// Основные функции
// -------------------------------------------------------------------------------------------------------
lsi_t list_insert_after(list_t* list,
                        lsi_t prev_index,
                        lsd_t value)
{
    assert(list);

    index_t* index_data = list->index_inf;
    lsi_t new_index = list->free;

    list->free = index_data[new_index].next;
    index_data[list->free].prev = index_data[new_index].prev;
    index_data[index_data[new_index].prev].next = list->free;

    list->data[new_index] = value;

    index_data[new_index].next = index_data[prev_index].next;
    index_data[prev_index].next = new_index;
    index_data[new_index].prev = prev_index;
    index_data[index_data[new_index].next].prev = new_index;

    list->list_inf.current_size++;

    return new_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
lsi_t list_delete_current(list_t* list,
                          lsi_t del_index)
{
    assert(list);

    index_t* index_data = list->index_inf;
    lsi_t prev_del_index = index_data[del_index].prev;

    index_data[prev_del_index].next = index_data[del_index].next;
    index_data[index_data[del_index].next].prev = prev_del_index;
    list->data[del_index] = L_FREE_NUM;

    index_data[del_index].next = list->free;
    index_data[del_index].prev = index_data[list->free].prev;
    index_data[index_data[list->free].prev].next = del_index;
    index_data[list->free].prev = del_index;
    list->free = del_index;

    list->list_inf.current_size--;

    return prev_del_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_print(list_t* list)
{
    assert(list);

    printf("INDEX  | DATA   NEXT   PREV\n");
    for (size_t i = 0; i < list->list_inf.capacity; i++)
    {
        printf("[%4zu] | [" DUMP_FORMAT_LSD_T "] [" DUMP_FORMAT_LSI_T "] [" DUMP_FORMAT_LSI_T "]\n",
               i, list->data[i], list->index_inf[i].next, list->index_inf[i].prev);
    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------



// Функции для пользователя
// -------------------------------------------------------------------------------------------------------
int def_insertAfter(list_t* list,
                    lsi_t prev_index,
                    lsd_t value,
                    const char* name_func,
                    const char* name_file,
                    int number_line)
{
    assert(list);

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

    int return_index = list_insert_after(list, prev_index, value);

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

    int return_index = list_delete_current(list, del_index);

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

    int return_index = list_delete_current(list, list->index_inf[del_index].next);

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

    list_push_error_inf(list, name_file, number_line, name_func);

    char buffer[200] = "DUMP before printList";

    list_dump_log(list, NAME_LOG_FILE, buffer);
    list_dump_html(list, NAME_DUMP_FILE, buffer);

    list_print(list);

    return 0;
}
// -------------------------------------------------------------------------------------------------------


//!Вроде сделан
// Проверки
// -------------------------------------------------------------------------------------------------------
int list_verifier(list_t* list)
{
    assert(list);
    int return_error = list->list_inf.error_inf.current_error;
    // printf("ERROR 1: %d\n", return_error);

    if (list->data == NULL)
    {
        return_error |= L_NULL_DATA;
    }

    if (list->index_inf == NULL)
    {
        return_error |= L_NULL_INDEX;
    }

    if ((size_t) list->free >= list->list_inf.capacity)
    {
        return_error |= L_BIG_FREE;
    }

    // printf("ERROR 2: %d\n", return_error);

    if (list->list_inf.current_size >= list->list_inf.capacity)
    {
        return_error |= L_SIZE_B_CAPACITY;
    }

    // Дальше нам необходимо использовать NEXT, PREV и FREE
    if ((return_error & L_NULL_DATA) == L_NULL_DATA ||
        (return_error & L_NULL_INDEX) == L_NULL_INDEX ||
        (return_error & L_BIG_FREE) == L_BIG_FREE)
    {
        list->list_inf.error_inf.current_error |= return_error;
        return return_error;
    }

    // printf("ERROR 3: %d\n", return_error);
    lsi_t* main_index = (lsi_t*) calloc(list->list_inf.current_size, sizeof(lsi_t));
    if (main_index == NULL)
    {
        return_error |= L_CALLOC_NULL;
        return return_error;
    }

    lsi_t index = list->index_inf[0].next;
    for (size_t i = 0; i < list->list_inf.current_size; i++)
    {
        if ((size_t) index >= list->list_inf.capacity)
        {
            return_error |= L_BIG_INDEX;
            break;
        }

        if ((size_t) list->index_inf[index].next >= list->list_inf.capacity)
        {
            return_error |= L_BIG_INDEX;
            break;
        }

        if (index != list->index_inf[list->index_inf[index].next].prev)
        {
            return_error |= L_BAD_INDEX_ORDER;
            break;
        }
        main_index[i] = index;
        index = list->index_inf[index].next;
    }

    if (index != 0)
    {
        return_error |= L_BAD_INDEX_ORDER;
    }

    index = list->free;
    for (size_t i = 0; i < list->list_inf.capacity - list->list_inf.current_size - 1; i++)
    {
        // printf("INDEX: [%d] NEXT: [%d]\n", index, list->index_inf[index].next);
        if ((size_t) index >= list->list_inf.capacity)
        {
            return_error |= L_BIG_FREE;
            break;
        }

        if ((size_t) list->index_inf[index].next >= list->list_inf.capacity)
        {
            return_error |= L_BIG_FREE;
            break;
        }

        if (index != list->index_inf[list->index_inf[index].next].prev)
        {
            return_error |= L_BAD_FREE_ORDER;
            break;
        }

        for (size_t idx_main = 0; idx_main < list->list_inf.current_size; idx_main++)
        {
            if (index == main_index[idx_main])
            {
                return_error |= L_MIX_INDEX_FREE;
                break;
            }
        }
        index = list->index_inf[index].next;
    }
    // printf("INDEX: %d FREE: %d\n", index, list->free);
    if (index != list->free)
    {
        return_error |= L_BAD_FREE_ORDER;
    }

    free(main_index);

    list_print_error(list);

    list->list_inf.error_inf.current_error |= return_error;
    return return_error;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_push_error_inf(list_t* list,
                        const char* name_file,
                        int number_line,
                        const char* name_func)
{
    assert(list);

    list->list_inf.error_inf.name_call_file = name_file;
    list->list_inf.error_inf.name_call_func = name_func;
    list->list_inf.error_inf.number_call_line = number_line;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_print_error(list_t* list)
{
    assert(list);

    int error = list->list_inf.error_inf.current_error;

    if (error != L_NOT_ERRORS)
    {
        printf("ERRORS:\n");

        for (int i = 1; i <= MAX_ERROR; i *= 2)
        {
            if ((list->list_inf.error_inf.current_error & i) == i)
            {
                printf("    [" FORMAT_ERROR "]\n", (unsigned) i);
            }
        }
    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------



// Дампы
// -------------------------------------------------------------------------------------------------------
list_error_t list_dump_log(list_t* list,
                           const char* name_file,
                           const char* dump_reason)
{
    assert(list);
    assert(name_file);
    // assert(dump_reason); НЕ НУЖЕН

    FILE* file = fopen_file(name_file, "a");

    fprintf(file, "-----------------------------------------------------------------------------------\n");

    fprintf(file,
            "Dump has called function \"%s\" in \"%s:%d\"\n",
            list->list_inf.error_inf.name_call_func,
            list->list_inf.error_inf.name_call_file,
            list->list_inf.error_inf.number_call_line);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long current_time = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000) % TIME_DIV;
    fprintf(file,
            "Time after start: [%lld] miliseconds\n",
            current_time - list->list_inf.create_inf.time_start);

    fprintf(file, "Reason to dump: %s\n", dump_reason);

    if (list->list_inf.error_inf.current_error == L_NOT_ERRORS)
    {
        fprintf(file, "Not errors in list\n");
    }
    else
    {
        fprintf(file, "Errors with list:\n");

        size_t counter = 1;
        int error_func = list->list_inf.error_inf.current_error;
        // printf("ERROR: %d\n", error_func);

        for (int error = 1; error <= MAX_ERROR; error *= 2)
        {
            if ((error_func & error) == error)
            {
                fprintf(file, "    %s\n", DISC_ERROR[counter]);
            }
            counter++;
        }
    }

    fprintf(file, "List stats:\n    Name list: \"%s\"\n    Place creation \"%s:%d\"\n    ----------------\n",
             list->list_inf.create_inf.name_list,
             list->list_inf.create_inf.name_file,
             list->list_inf.create_inf.number_line);
    fprintf(file, "    Head: [" DUMP_FORMAT_LSI_T "]\n    Tail: [" DUMP_FORMAT_LSI_T "]\n    Free: [" DUMP_FORMAT_LSI_T "]\n",
            list->index_inf[0].next,
            list->index_inf[0].prev,
            list->free);

    if ((list->list_inf.error_inf.current_error & L_NULL_DATA) != L_NULL_DATA ||
        (list->list_inf.error_inf.current_error & L_NULL_INDEX) != L_NULL_INDEX)
    {
        fprintf(file, "INDEX  | DATA       NEXT       PREV\n");
        for (size_t i = 0; i < list->list_inf.capacity; i++)
        {
            fprintf(file,
                    "[%4zu] | [" DUMP_FORMAT_LSD_T "] [" DUMP_FORMAT_LSI_T "] [" DUMP_FORMAT_LSI_T "]\n",
                    i, list->data[i], list->index_inf[i].next, list->index_inf[i].prev);
        }
    }

    fprintf(file, "-----------------------------------------------------------------------------------\n");
    fclose_file(file);

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int clean_log_file(void)
{
    FILE* buffer = fopen_file(NAME_LOG_FILE, "w");
    fclose_file(buffer);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_head_html_file(void)
{
    FILE* html_file = fopen_file(NAME_DUMP_FILE, "w");

    fprintf(html_file, "<pre style=\"font-family: 'Courier New', monospace; font-size: 14px; color: #e0e0e0; background-color: #1e1e1e; padding: 10px; border-radius: 6px;\">\n\n");
    fclose_file(html_file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t create_graph(list_t* list)
{
    assert(list);

    // if (list->list_inf.error_inf.current_error != 0)
    // {
    //     return L_NOT_ERRORS;
    // }

    FILE* dot_file = fopen_file(NAME_DOT_FILE, "w");

    fprintf(dot_file,
            "digraph {\n"
            "  rankdir=LR;\n"
            "  bgcolor=\"#1e1e1e\""
            // "  splines=ortho;\n"
            "  nodesep=0.4;\n"
            "  ranksep=0.6;\n"
            "  node [shape=Mrecord, style=filled, fontname=\"Helvetica\"];\n"
            "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    // fprintf(dot_file,
            // "head [label=\"HEAD\", shape=box, fillcolor=\"#b81882ff\"]\n"
            // "tail [label=\"TAIL\", shape=box, fillcolor=\"#cac711ff\"]\n\n");

    lsi_t* mass_index_inf = (lsi_t*) calloc(list->list_inf.capacity, sizeof(lsi_t));
    if (mass_index_inf == NULL)
    {
        printf("ERROR: calloc return null address in create_graph\n");
        return L_CALLOC_NULL;
    }

    lsi_t* mass_index_free = (lsi_t*) calloc(list->list_inf.capacity, sizeof(lsi_t));
    if (mass_index_free == NULL)
    {
        printf("ERROR: calloc return null address in create_graph\n");
        return L_CALLOC_NULL;
    }

    size_t count_el_inf = 0;
    size_t count_el_free = 0;

    lsi_t index = 0;
    for (size_t i = 0; i < list->list_inf.current_size + 1; i++)
    {
        if ((size_t) index >= list->list_inf.capacity)
        {
            break;
        }

        if ((size_t) list->index_inf[index].next >= list->list_inf.capacity)
        {
            break;
        }

        if (index != list->index_inf[list->index_inf[index].next].prev)
        {
            break;
        }

        mass_index_inf[count_el_inf++] = index;
        index = list->index_inf[index].next;
    }

    index = list->free;
    for (size_t i = 0; i < list->list_inf.capacity - list->list_inf.current_size - 1; i++)
    {
        if ((size_t) index >= list->list_inf.capacity)
        {
            break;
        }

        if ((size_t) list->index_inf[index].next >= list->list_inf.capacity)
        {
            break;
        }

        if (index != list->index_inf[list->index_inf[index].next].prev)
        {
            break;
        }

        mass_index_free[count_el_free++] = index;
        index = list->index_inf[index].next;
    }

    for (size_t i = 0; i < list->list_inf.capacity; i++)
    {
        const char* color = "";

        if ((int)  i == 0)
        {
            color = "#696969ff";
        }

        if (color[0] == '\0')
        {
            if ((int) i == list->index_inf[0].next)
            {
                color = "#cc4ec1ff";
            }

            else if ((int) i == list->index_inf[0].prev)
            {
                color = "#d8d535ff";
            }
        }

        if (color[0] == '\0')
        {
            for (size_t idx = 0; idx < count_el_inf; idx++)
            {
                if ((int) i == mass_index_inf[idx])
                {
                    color = "#2ab8dbff";
                    break;
                }
            }
        }

        if (color[0] == '\0')
        {
            for (size_t idx = 0; idx < count_el_free; idx++)
            {
                if ((int) i == mass_index_free[idx])
                {
                    color = "#56e65dff";
                    break;
                }
            }
        }

        if (color[0] == '\0')
        {
            color = "#e94747ff";
        }

        fprintf(dot_file,
                "block%zu [label=\"INDEX=%zu|DATA=" FORMAT_LSD_T "|NEXT=" FORMAT_LSI_T "|PREV=" FORMAT_LSI_T "\", fillcolor=\"%s\"];\n",
                i, i, list->data[i], list->index_inf[i].next, list->index_inf[i].prev, color);

    }

    for (size_t i = 0; i < list->list_inf.capacity - 1; i++)
    {
        fprintf(dot_file, "block%zu -> block%zu [style=invis, weight=100];\n", i, i + 1);
    }

    fprintf(dot_file, "\n");

    for (size_t i = 0; i < count_el_inf; i++)
    {
        fprintf(dot_file,
                "block" FORMAT_LSI_T " -> block" FORMAT_LSI_T " [color=\"#382dd1ff\", penwidth=1.5, arrowsize=0.6, constraint=true, dir = both];\n",
                mass_index_inf[i], list->index_inf[mass_index_inf[i]].next);
    }

    for (size_t i = 0; i < count_el_free; i++)
    {
        // printf("INDEX = %d\n", mass_index_free[i]);
        fprintf(dot_file,
                "block" FORMAT_LSI_T " -> block" FORMAT_LSI_T " [color=\"#1dad10ff\", penwidth=1.5, arrowsize=0.6, constraint=true, dir = both];\n",
                mass_index_free[i], list->index_inf[mass_index_free[i]].next);
    }

    free(mass_index_inf);
    free(mass_index_free);


    fprintf(dot_file,
            "head -> block%d [color=\"#cc4ec1ff\", penwidth=1.5, arrowsize=0.7];\n"
            "tail -> block%d [color=\"#FFA500\", penwidth=1.5, arrowsize=0.7];\n"
            "free -> block%d [color=\"#00A500\", penwidth=1.5, arrowsize=0.7];\n",
            list->index_inf[0].next,
            list->index_inf[0].prev,
            list->free);

    fprintf(dot_file, "}\n");

    fclose_file(dot_file);

    char buffer_name_file[100] = {};
    sprintf(buffer_name_file, "%s%d.png", ADDRESS_IMG_FILES, AMOUNT_IMAGES);

    char system_cmd[100] = {};
    sprintf(system_cmd, "dot %s -T png -o %s", NAME_DOT_FILE, buffer_name_file);
    int trash = system(system_cmd);
    (void) trash;

    AMOUNT_IMAGES++;

    return L_NOT_ERRORS;
}
// -----------------------------------------------  --------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_dump_html(list_t* list,
                   const char* name_html_file,
                   const char* dump_reason)
{
    assert(list);
    assert(name_html_file);

    FILE* html_file = fopen(name_html_file, "a");

    fprintf(html_file, "-----------------------------------------------------------------------------------\n");

    fprintf(html_file,
            "\n<mark style=\"background-color: #1808adff; color: #e0e0e0\">Dump has called function \"%s\" in \"%s:%d\"</mark>\n",
            list->list_inf.error_inf.name_call_func,
            list->list_inf.error_inf.name_call_file,
            list->list_inf.error_inf.number_call_line);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long current_time = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000) % TIME_DIV;
    fprintf(html_file,
            "<mark style=\"background-color: #1808adff; color: #e0e0e0\">Time after start: [%lld] miliseconds</mark>\n",
            current_time - list->list_inf.create_inf.time_start);

    fprintf(html_file, "<mark style=\"background-color: #1808adff; color: #e0e0e0\">Reason to dump: %s</mark>", dump_reason);

    if (list->list_inf.error_inf.current_error == L_NOT_ERRORS)
    {
        fprintf(html_file, "<mark style=\"background-color: #3aaa06ff; color: #e0e0e0\">Not errors in list</mark>\n");
    }
    else
    {
        fprintf(html_file, "<mark style=\"background-color: #cc0505ff; color: #e0e0e0\">Errors with list:</mark>\n");

        size_t counter = 1;
        int error_func = list->list_inf.error_inf.current_error;
        // printf("ERROR: %d\n", error_func);

        for (int error = 1; error <= MAX_ERROR; error *= 2)
        {
            if ((error_func & error) == error)
            {
                fprintf(html_file, "<mark style=\"background-color: #cc0505ff; color: #e0e0e0\">    %s</mark>\n", DISC_ERROR[counter]);
            }
            counter++;
        }
    }

    fprintf(html_file, "<mark style=\"background-color: #1808adff; color: #e0e0e0\">List stats:\n    Name list: \"%s\"\n    Place creation \"%s:%d\"\n    ----------------\n",
             list->list_inf.create_inf.name_list,
             list->list_inf.create_inf.name_file,
             list->list_inf.create_inf.number_line);
    fprintf(html_file, "    Head: [" DUMP_FORMAT_LSI_T "]\n    Tail: [" DUMP_FORMAT_LSI_T "]\n    Free: [" DUMP_FORMAT_LSI_T "]</mark>\n",
            list->index_inf[0].next,
            list->index_inf[0].prev,
            list->free);

    if ((list->list_inf.error_inf.current_error & L_NULL_DATA) != L_NULL_DATA ||
        (list->list_inf.error_inf.current_error & L_NULL_INDEX) != L_NULL_INDEX)
    {
        fprintf(html_file,
                "<mark style=\"background-color: #9509b8ff; color: #e0e0e0\">INDEX </mark> | "
                "<mark style=\"background-color: #3b2dbeff; color: #e0e0e0\">DATA      </mark> "
                "<mark style=\"background-color: #b60f0fff; color: #e0e0e0\">NEXT      </mark> "
                "<mark style=\"background-color: #c78c0cff; color: #e0e0e0\">PREV      </mark> \n");
        for (size_t i = 0; i < list->list_inf.capacity; i++)
        {
            fprintf(html_file,
                    "<mark style=\"background-color: #9509b8ff; color: #e0e0e0\">[%4zu]</mark>"
                    " | <mark style=\"background-color: #3b2dbeff; color: #e0e0e0\">[" DUMP_FORMAT_LSD_T "]</mark>"
                    " <mark style=\"background-color: #b60f0fff; color: #e0e0e0\">[" DUMP_FORMAT_LSI_T "]</mark>"
                    " <mark style=\"background-color: #c78c0cff; color: #e0e0e0\">[" DUMP_FORMAT_LSI_T "]</mark>\n",
                    i, list->data[i], list->index_inf[i].next, list->index_inf[i].prev);
        }
    }

    // fprintf(html_file, "-----------------------------------------------------------------------------------\n");

    create_graph(list);

    char address[100] = {};
    sprintf(address, "%s%d.png", ADDRESS_IMG_HTML, AMOUNT_IMAGES - 1);
    // printf("ADDRESS: %s\n", address);
    fprintf(html_file, "<img src=%s width=2000px>\n", address);

    fclose_file(html_file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------



// Дополнительные функции
// -------------------------------------------------------------------------------------------------------
lsi_t list_tail(list_t* list)
{
    assert(list);
    return list->index_inf[0].prev;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
lsi_t list_head(list_t* list)
{
    assert(list);
    return list->index_inf[0].next;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_error(list_t* list)
{
    assert(list);
    return list->list_inf.error_inf.current_error;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// lsi_t
