#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "LIB/my-list.h"

#include "../COMMON/support.h"
#include "../COMMON/color.h"


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

    for (int i = 1; i < START_SIZE_LIST; i++)
    {
        buf_index[i].next = i + 1;
        buf_index[i].prev = -1;
    }
    buf_index[0].next = 0;
    buf_index[0].prev = 1;
    buf_index[START_SIZE_LIST - 1].next = 0;

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
    clean_dir_images();
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
lsi_t  list_insert_after(list_t* list,
                        lsi_t prev_index,
                        lsd_t value)
{
    assert(list);

    index_t* index_data = list->index_inf;
    lsi_t new_index = list->free;
    list->free = index_data[new_index].next;
    // TODO Сделать реалокацию сразу после проверки

    index_data[new_index].next = index_data[prev_index].next;
    index_data[new_index].prev = prev_index;
    index_data[index_data[new_index].next].prev = new_index;
    index_data[prev_index].next = new_index;

    list->data[new_index] = value;
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
    index_data[del_index].prev = prev_del_index;
    index_data[del_index].next = list->free;

    index_data[del_index].prev = -1;
    list->data[del_index] = L_FREE_NUM;
    list->free = del_index;

    list->list_inf.current_size--;

    return prev_del_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_print(list_t* list)
{
    assert(list);

    // if (listError(list) != L_NULL_DATA || listError(list) != L_NULL_INDEX)
    printf("INDEX  | DATA   NEXT   PREV\n");
    for (size_t i = 0; i < list->list_inf.capacity; i++)
    {
        printf("[%4zu] | [" DUMP_FORMAT_LSD_T "] [" DUMP_FORMAT_LSI_T "] [" DUMP_FORMAT_LSI_T "]\n",
               i, list->data[i], list->index_inf[i].next, list->index_inf[i].prev);
    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_sort(list_t* list)
{
    assert(list);

    index_t* index_data = list->index_inf;
    lsd_t* mass_data = (lsd_t*) calloc(list->list_inf.current_size, sizeof(lsd_t));
    if (mass_data == NULL)
    {
        list->list_inf.error_inf.current_error |= L_CALLOC_NULL;
        return -1;
    }

    lsi_t index = index_data[0].next;
    for (size_t i = 0; i < list->list_inf.current_size; i++, index = index_data[index].next)
    {
        mass_data[i] = list->data[index];
    }
    size_t size = list->list_inf.current_size;

    sort_data(mass_data, size);

    index_data[0].next = 1;
    index_data[0].prev = (lsd_t) size;

    for (size_t i = 1; i < size + 1; i++)
    {
        list->data[i] = mass_data[i - 1];
        index_data[i].prev = (lsd_t) i - 1;
        index_data[i].next = (lsd_t) i + 1;
    }
    index_data[size].next = 0;
    free(mass_data);

    for (size_t i = size + 1; i < list->list_inf.capacity; i++)
    {
        index_data[i].next = (lsd_t) i + 1;
        index_data[i].prev = -1;
        list->data[i] = L_FREE_NUM;
    }
    index_data[list->list_inf.capacity - 1].next = 0;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int compare_func(const void* void_data_1,
                 const void* void_data_2)
{
    assert(void_data_1);
    assert(void_data_2);

    const lsd_t* data_1 = (const lsd_t*) void_data_1;
    const lsd_t* data_2 = (const lsd_t*) void_data_2;

    return (*data_1 > *data_2) - (*data_1 < *data_2);
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int sort_data(lsd_t* mass_data,
              size_t amount_el)
{
    assert(mass_data);

    qsort(mass_data,
          amount_el,
          sizeof(lsd_t),
          &compare_func);

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
lsi_t list_next(list_t* list, lsi_t index)
{
    assert(list);

    if ((size_t) index >= list->list_inf.capacity)
    {
        return -1;
    }

    lsi_t ret_value = list->index_inf[index].next;

    if ((size_t) ret_value >= list->list_inf.capacity)
    {
        return -1;
    }

    return ret_value;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
lsi_t list_prev(list_t* list, lsi_t index)
{
    assert(list);

    if ((size_t) index >= list->list_inf.capacity)
    {
        return -1;
    }

    lsi_t ret_value = list->index_inf[index].prev;

    if ((size_t) ret_value >= list->list_inf.capacity)
    {
        return -1;
    }

    return ret_value;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
lsd_t list_value(list_t* list, lsi_t index)
{
    assert(list);

    if ((size_t) index >= list->list_inf.capacity)
    {
        return L_ERR_VALUE;
    }

    return list->data[index];
}
// -------------------------------------------------------------------------------------------------------

