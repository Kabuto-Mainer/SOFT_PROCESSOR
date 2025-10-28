#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list-type.h"
#include "list-config.h"
#include "list-const.h"
#include "list-func.h"

#include "../COMMON/support.h"
#include "../COMMON/color.h"

static int AMOUNT_IMAGES = 0;

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

    for (int i = 1; i < START_SIZE_LIST - 1; i++)
    {
        buf_index[i].next = i + 1;
        buf_index[i].prev = i - 1;
    }
    buf_index[0].next = 0;
    buf_index[0].prev = 0;
    buf_index[1].prev = 0;


    buf_index[START_SIZE_LIST - 1].next = 0;
    buf_index[START_SIZE_LIST - 1].prev = START_SIZE_LIST - 2;

    for (int i = 1; i < START_SIZE_LIST; i++)
    {
        buf_data[i] = -1;
    }
    buf_data[0] = 0;

    list->free = 1;
    list->tail = 0;
    list->head = 0;

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
    list_inf->current_size = START_SIZE_LIST;

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

// -------------------------------------------------------------------------------------------------------
list_error_t list_insert_after(list_t* list,
                               lsi_t prev_index,
                               lsd_t value)
{
    assert(list);

    COMMON_VERIFIER(list, CHAR_FUNC_INSERT_AFTER, L_ERROR, "dump before insert");

    index_t* index_data = list->index_inf;
    lsi_t new_index = list->free;
    list->free = index_data[new_index].next;
    index_data[list->free].prev = L_FREE_NUM;

    list->data[new_index] = value;
    index_data[new_index].next = index_data[prev_index].next;
    index_data[prev_index].next = new_index;
    index_data[new_index].prev = prev_index;


    if (index_data[new_index].next == 0)
    {
        list->tail = new_index;
    }
    else
    {
        index_data[index_data[new_index].next].prev = new_index;
    }

    if (prev_index == 0)
    {
        index_data[0].next = 0;
        index_data[new_index].next = list->head;
    }

    if (index_data[new_index].prev == 0)
    {
        list->head = new_index;
    }

    COMMON_VERIFIER(list, CHAR_FUNC_INSERT_AFTER, L_ERROR, "dump after insert");

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------
//
// // -------------------------------------------------------------------------------------------------------
// list_error_t list_append(list_t* list,
//                          lsd_t value)
// {
//     assert(list);
//
//     // printf("LIST APPEND\n");
//     COMMON_VERIFIER(list, CHAR_FUNC_APPEND, L_ERROR);
//
//     index_t* index_data = list->index_inf;
//     lsi_t new_index = list->free;
//     list->data[new_index] = value;
//
//     index_data[list->tail].next = new_index;
//     if (list->tail == 0)
//     {
//         index_data[0].next = 0;
//         list->head = new_index;
//     }
//
//     index_data[new_index].prev = list->tail;
//     list->tail = new_index;
//     list->free = index_data[new_index].next;
//     index_data[list->free].prev = L_FREE_NUM;
//     index_data[new_index].next = 0;
//
//
//     COMMON_VERIFIER(list, CHAR_FUNC_APPEND, L_ERROR);
//
//     return L_NOT_ERRORS;
// }
// // -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_delete_current(list_t* list,
                                 lsi_t del_index)
{
    assert(list);

    COMMON_VERIFIER(list, CHAR_FUNC_DELETE_CURRENT, L_ERROR, "dump before delete");

    index_t* index_data = list->index_inf;
    lsi_t prev_del_index = index_data[del_index].prev;

    index_data[prev_del_index].next = index_data[del_index].next;
    index_data[index_data[del_index].next].prev = prev_del_index;
    index_data[0].prev = 0;

    if (list->tail == del_index)
    {
        index_data[prev_del_index].next = 0;
        list->tail = prev_del_index;
    }

    list->data[del_index] = L_FREE_NUM;

    index_data[del_index].next = list->free;
    index_data[list->free].prev = del_index;
    index_data[del_index].prev = 0;

    COMMON_VERIFIER(list, CHAR_FUNC_DELETE_CURRENT, L_ERROR, "dump after delete");

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_delete_after(list_t* list,
                               lsi_t before_del_index)
{
    assert(list);

    COMMON_VERIFIER(list, CHAR_FUNC_DELETE_AFTER, L_ERROR, "dump before delete");

    index_t* index_data = list->index_inf;
    lsi_t del_index = index_data[before_del_index].next;

    list->data[del_index] = L_FREE_NUM;

    index_data[before_del_index].next = index_data[del_index].next;
    index_data[index_data[del_index].next].prev = before_del_index;
    index_data[0].prev = 0;

    if (del_index == list->tail)
    {
        list->tail = before_del_index;
    }

    index_data[list->free].prev = del_index;
    index_data[del_index].next = list->free;
    index_data[del_index].prev = 0;
    list->free = del_index;

    COMMON_VERIFIER(list, CHAR_FUNC_DELETE_AFTER, L_ERROR, "dump after delete");

    return L_NOT_ERRORS;
}
//
// // Необходим int
// // -------------------------------------------------------------------------------------------------------
int list_verifier(list_t* list,
                  const char* name_file,
                  const int number_line,
                  const char* name_func)
{
    assert(list);

    list->list_inf.error_inf.name_call_file = name_file;
    list->list_inf.error_inf.name_call_func = name_func;
    list->list_inf.error_inf.number_call_line = number_line;

    int return_error = list->list_inf.error_inf.current_error;

    if (list->data == NULL)
    {
        return_error |= L_NULL_DATA;
    }

    if (list->index_inf == NULL)
    {
        return_error |= L_NULL_INDEX;
    }

    if (list->data[list->free] !=  L_FREE_NUM)
    {
        return_error |= L_BAD_FREE;
    }

    // Мы можем преобразовывать в size_t, т.к. при отрицательных числах будет переполнение и мы получим очень большое число
    if ((size_t) list->tail >= list->list_inf.current_size ||
        (size_t) list->head >= list->list_inf.current_size)
    {
        return_error |= L_BAD_TAIL;
    }

    if (list->index_inf[list->tail].next != 0)
    {
        return_error |= L_BAD_TAIL;
    }

    if (list->index_inf[list->head].prev != 0)
    {
        return_error |= L_BAD_HEAD;
    }

    // Дальше нам необходимо использовать NEXT и prev
    if ((return_error & L_NULL_DATA) == L_NULL_DATA ||
        (return_error & L_NULL_INDEX) == L_NULL_INDEX)
    {
        list->list_inf.error_inf.current_error = return_error;
        return return_error;
    }

    if ((return_error & L_BAD_TAIL) == L_BAD_TAIL ||
        (return_error & L_BAD_HEAD) == L_BAD_HEAD)
    {
        list->list_inf.error_inf.current_error = return_error;
        return return_error;
    }

    for (lsi_t index = list->head;
         index != list->tail;
         index = list->index_inf[index].next)
    {
        if ((size_t) index > list->list_inf.current_size)
        {
            return_error |= L_TOO_BIG_INDEX;
            break;
        }

        if (index != list->index_inf[list->index_inf[index].next].prev)
        {
            return_error |= L_BAD_INDEX_INF;
            break;
        }
    }

    for (lsi_t index = list->free;
         list->index_inf[index].next != 0;
         index = list->index_inf[index].next)
    {
        if ((size_t) index > list->list_inf.current_size)
        {
            return_error |= L_TOO_BIG_INDEX;
            break;
        }

        if (index != list->index_inf[list->index_inf[index].next].prev)
        {
            return_error |= L_BAD_INDEX_FREE;
            break;
        }
    }

    list->list_inf.error_inf.current_error = return_error;
    list->list_inf.error_inf.name_call_file = name_file;
    list->list_inf.error_inf.name_call_func = name_func;
    list->list_inf.error_inf.number_call_line = number_line;

    return return_error;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_check_insert(list_t* list,
                               lsi_t index,
                               lsd_t value)
{
    assert(list);
    (void) value;

    if ((size_t) index > list->list_inf.current_size)
    {
        list->list_inf.error_inf.current_error |= L_INSERT_INDEX;
    }

    if (list->free == 0)
    {
        list->list_inf.error_inf.current_error |= L_BAD_SIZE;
    }

    if (list->data[index] == L_FREE_NUM && index != 0)
    {
        list->list_inf.error_inf.current_error |= L_INSERT_INDEX;
    }

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_check_del(list_t* list,
                            lsi_t index)
{
    assert(list);

    if (index < 0 || (size_t) index > list->list_inf.current_size)
    {
        list->list_inf.error_inf.current_error |= L_BAD_DEL;
    }

    if (list->data[index] == -1)
    {
        list->list_inf.error_inf.current_error |= L_BAD_DEL;
    }

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_dump(list_t* list,
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

        for (int error = 1; error <= MAX_ERROR; error *= 2)
        {
            if ((error_func & error) == error)
            {
                fprintf(file, "    %s\n", DISC_ERROR[counter]);
            }
            counter++;
        }
    }

    fprintf(file, "List stats:\n  Name list: \"%s\"\n  Place creation \"%s:%d\"\n    ---------------\n",
             list->list_inf.create_inf.name_list,
             list->list_inf.create_inf.name_file,
             list->list_inf.create_inf.number_line);
    fprintf(file, "  Head: [%04d]\n  Tail: [%04d]\n  Free: [%04d]\n",
            list->head, list->tail, list->free);

    if ((list->list_inf.error_inf.current_error & L_NULL_DATA) != L_NULL_DATA ||
        (list->list_inf.error_inf.current_error & L_NULL_INDEX) != L_NULL_INDEX)
    {
        fprintf(file, "List:\nINDEX  |  DATA   NEXT   PREV\n");

        for (size_t i = 0; i < list->list_inf.current_size; i++)
        {
            fprintf(file, "[%04zu] | [%04d] [%04d] [%04d]\n",
                    i,
                    list->data[i],
                    list->index_inf[i].next,
                    list->index_inf[i].prev);
        }
    }

    fprintf(file, "-----------------------------------------------------------------------------------\n");

    fclose_file(file);

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t create_graph(list_t* list)
{
    assert(list);

    if (list->list_inf.error_inf.current_error != 0)
        return L_NOT_ERRORS;

    FILE* dot_file = fopen_file(NAME_DOT_FILE, "w");

    fprintf(dot_file,
        "digraph {\n"
        "  rankdir=LR;\n"
        "  splines=ortho;\n"
        "  nodesep=0.4;\n"
        "  ranksep=0.6;\n"
        "  node [shape=Mrecord, style=filled, fontname=\"Helvetica\"];\n"
        "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    for (size_t i = 0; i < list->list_inf.current_size; i++)
    {
        const char* color;
        if (i == 0)
            color = "#FF0000";
        else if (list->data[i] == L_FREE_NUM)
            color = "#A0FFA0";
        else
            color = "#FFC0C0";

        fprintf(dot_file,
            "block%zu [label=\"INDEX=%zu|DATA=%d|NEXT=%d|PREV=%d\", fillcolor=\"%s\"];\n",
            i, i, list->data[i], list->index_inf[i].next, list->index_inf[i].prev, color);
    }

    fprintf(dot_file,
        "head [label=\"HEAD=%d\", shape=box, style=filled, fillcolor=\"#FF6666\"];\n"
        "tail [label=\"TAIL=%d\", shape=box, style=filled, fillcolor=\"#FFFF66\"];\n\n",
        list->head, list->tail);

    for (size_t i = 0; i < list->list_inf.current_size - 1; i++)
    {
        fprintf(dot_file, "block%zu -> block%zu [style=invis, weight=100];\n", i, i + 1);
    }
    fprintf(dot_file, "\n");

    for (lsi_t index = list->head; index != 0 && list->index_inf[index].next != 0; index = list->index_inf[index].next)
    {
        fprintf(dot_file,
            "block%d -> block%d [color=\"#00D000\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
            index, list->index_inf[index].next);
    }

    for (lsi_t index = list->head; index != 0 && list->index_inf[index].next != 0; index = list->index_inf[index].next)
    {
        fprintf(dot_file,
            "block%d -> block%d [color=\"#D00000\", penwidth=1.5, arrowsize=0.5, constraint=false];\n",
            list->index_inf[index].next, index);
    }

    fprintf(dot_file,
        "head -> block%d [color=\"#FF0000\", penwidth=1.5, arrowsize=0.7];\n"
        "tail -> block%d [color=\"#FFA500\", penwidth=1.5, arrowsize=0.7];\n",
        list->head, list->tail);

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
int list_dump_html(list_t* list,
                   const char* name_html_file,
                   const char* dump_reason)
{
    assert(list);
    assert(name_html_file);

    FILE* html_file = fopen(name_html_file, "a");


    list_dump(list, name_html_file, dump_reason);
    create_graph(list);

    char address[100] = {};
    sprintf(address, "%s%d.png", ADDRESS_IMG_HTML, AMOUNT_IMAGES - 1);
    printf("ADDRESS: %s\n", address);
    fprintf(html_file, "<img src=%s width=1000px>\n", address);

    fclose_file(html_file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_head_html_file(void)
{
    FILE* html_file = fopen_file(NAME_DUMP_FILE, "w");

    fprintf(html_file, "<pre>\n");

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t list_print(list_t* list)
{
    assert(list);

    COMMON_VERIFIER(list, CHAR_FUNC_PRINT, L_ERROR, "dump before print");

    int counter = 0;
    for (lsi_t i = list->head;
         i != 0;
         i = list->index_inf[i].next)
    {
        printf("[%d]: [%d]\n", counter++, list->data[i]);
    }

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

