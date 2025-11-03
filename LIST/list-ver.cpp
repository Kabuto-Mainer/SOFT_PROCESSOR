#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "LIB/my-list.h"

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
        // printf("INDEX: [%d]\n", index);
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

        if (list->data[index] != L_FREE_NUM || list->index_inf[index].prev != -1)
        {
            return_error |= L_NO_CORRECT_FREE;
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
    if (index != 0)
    {
        return_error |= L_BAD_FREE_ORDER;
    }

    if (list->list_inf.current_size == list->list_inf.capacity && return_error == L_NOT_ERRORS)
    {
        list_realloc(list);
    }

    else if (list->list_inf.current_size >= list->list_inf.capacity)
    {
        return_error |= L_SIZE_B_CAPACITY;
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

// -------------------------------------------------------------------------------------------------------
int list_realloc(list_t* list)
{
    assert(list);

    size_t old_size = list->list_inf.capacity;
    lsd_t* buffer_lsd = (lsd_t*) realloc(list->data, sizeof(lsd_t) * old_size * MODE_REALLOC);
    index_t* buffer_index = (index_t*) realloc(list->index_inf, sizeof(index_t) * old_size * MODE_REALLOC);

    if (buffer_lsd == NULL || buffer_index == NULL)
    {
        list->list_inf.error_inf.current_error |= L_REALLOC_NULL;
        return -1;
    }

    for (size_t i = old_size; i < old_size * 2 - 1; i++)
    {
        buffer_index[i].prev = -1;
        buffer_lsd[i] = L_FREE_NUM;
        buffer_index[i].next = (int) i + 1;
    }

    buffer_index[old_size * MODE_REALLOC - 1].next = list->free;
    buffer_lsd[old_size * MODE_REALLOC - 1] = L_FREE_NUM;

    list->data = buffer_lsd;
    list->index_inf = buffer_index;
    list->list_inf.capacity *= MODE_REALLOC;

    return 0;
}
// -------------------------------------------------------------------------------------------------------


