#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "list-type.h"
#include "list-const.h"
#include "list-config.h"
#include "list-func.h"

#include "../COMMON/support.h"

static int AMOUNT_IMAGES = 0;


// Create and destroy
// -------------------------------------------------------------------------------------------------------
int list_ctr_main(list_t* list,
                  const char* name_file,
                  int number_line,
                  const char* name_list)
{
    assert(list);

    list->create_inf.name_file = name_file;
    list->create_inf.name_list = name_list;
    list->create_inf.number_line = number_line;
    list->error_inf.name_file = __FILE__;
    list->error_inf.number_line = __LINE__;
    list->error_inf.name_func = "list_ctr_main";

    list_node_t* buffer = (list_node_t*) calloc(1, sizeof(list_node_t));
    if (buffer == NULL)
    {
        list->error_inf.error = L_NULL_CALLOC;
        return -1;
    }

    list->fict = buffer;
    buffer->next = list->fict;
    buffer->prev = list->fict;
    buffer->data = 0;
    list->size = 0;

    list->error_inf.error = L_NOT_ERRORS;

    clean_dir_images();
    create_head_html_file();

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_dtr(list_t* list)
{
    assert(list);

    list_node_t* index = list->fict->next;
    list_node_t* buffer = NULL;
    for (size_t i = 0; i < list->size + 1; i++)
    {
        buffer = index;
        index = index->next;
        free(buffer);
    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------


// Main fuctions
// -------------------------------------------------------------------------------------------------------
list_node_t* list_ctr_el(list_node_t* prev,
                         list_node_t* next,
                         lsd_t value)
{
    list_node_t* buffer = (list_node_t*) calloc(1, sizeof(list_node_t));
    if (buffer == NULL)
    {
        return NULL;
    }

    buffer->data = value;
    buffer->next = next;
    buffer->prev = prev;

    next->prev = buffer;
    prev->next = buffer;

    return buffer;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_node_t* list_del_el(list_node_t* current)
{
// TODO  Ask DED: What must we do, if we got NULL address. Must we call destructor?

    current->next->prev = current->prev;
    current->prev->next = current->next;
    list_node_t* ret_address = current->prev;

    free(current);
    return ret_address;
}
// -------------------------------------------------------------------------------------------------------


// Standard fuctions for list
// -------------------------------------------------------------------------------------------------------
list_node_t* list_insert_after(list_t* list,
                               list_node_t* index,
                               lsd_t value,
                               const char* name_file,
                               int number_line,
                               const char* name_func)
{
    assert(list);

#if VERIFIER == ON

    fulling_error_inf(list, name_file, number_line, name_func);
    char reason[200] = "";

    if (index == NULL )
    {
        list->error_inf.error |= L_NULL_INPUT;
    }

    if (list_verifier(list) != L_NOT_ERRORS )
    {
        sprintf(reason, "ERROR in insertAfter before insert " FORMAT_DATA " after %p", value, index);
        list_dump(list, reason);
        return NULL;
    }
    sprintf(reason, "DUMP in insertAfter before insert " FORMAT_DATA " after %p", value, index);
    list_dump(list, reason);

#else
    (void) name_file;
    (void) number_line;
    (void) name_func;

#endif // VERIFIER == ON

    list_node_t* ret_index =  list_ctr_el(index, index->next, value);
    list->size++;

#if VERIFIER == ON

    if (list_verifier(list) != L_NOT_ERRORS )
    {
        sprintf(reason, "ERROR in insertAfter after insert " FORMAT_DATA " after %p", value, index);
        list_dump(list, reason);
        return NULL;
    }

    sprintf(reason, "DUMP in insertAfter after insert " FORMAT_DATA " after %p", value, index);
    list_dump(list, reason);

#endif // VERIFIER == ON

    return ret_index;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_node_t* list_delete_current(list_t* list,
                                 list_node_t* current_index,
                                 const char* name_file,
                                 int number_line,
                                 const char* name_func)
{
    assert(list);

#if VERIFIER == ON

    fulling_error_inf(list, name_file, number_line, name_func);
    char reason[200] = {};

    if (current_index == NULL)
    {
        list->error_inf.error |= L_NULL_INPUT;
    }

    if (list->size == 0)
    {
        list->error_inf.error |= L_BAD_SIZE;
    }

    if (list_verifier(list) != L_NOT_ERRORS)
    {
        sprintf(reason, "ERROR in deleteCurrent before delete %p", current_index);
        list_dump(list, reason);
        return NULL;
    }
    sprintf(reason, "DUMP in deleteCurrent before delete %p", current_index);
    list_dump(list, reason);

#else

    (void) name_file;
    (void) number_line;
    (void) name_func;

#endif // VERIFIER == ON

    list_node_t* ret_index = list_del_el(current_index);
    list->size--;

#if VERIFIER == ON

    if (list_verifier(list) != L_NOT_ERRORS)
    {
        sprintf(reason, "ERROR in deleteCurrent after delete %p", current_index);
        list_dump(list, reason);
    }
    sprintf(reason, "DUMP in deleteCurrent after delete %p", current_index);
    list_dump(list, reason);

#endif // VERIFIER == ON

    return ret_index;
}
// -------------------------------------------------------------------------------------------------------

// Just a little support fuction
// -------------------------------------------------------------------------------------------------------
int fulling_error_inf(list_t* list,
                      const char* name_file,
                      int number_line,
                      const char* name_func)
{
    assert(list);

    list->error_inf.name_file = name_file;
    list->error_inf.name_func = name_func;
    list->error_inf.number_line = number_line;

    return 0;
}
// -------------------------------------------------------------------------------------------------------


// All for debuging
// -------------------------------------------------------------------------------------------------------
int list_verifier(list_t* list)
{
    assert(list);

    if (list->fict == NULL)
    {
        list->error_inf.error |= L_NULL_FICT;
        return list->error_inf.error;
    }

    list_node_t* index = list->fict;
    for (size_t i = 0; i < list->size + 1; i++)
    {
        if (index == NULL || index->next == NULL)
        {
            list->error_inf.error |= L_NULL_EL;
            break;
        }

        if (index != index->next->prev)
        {
            list->error_inf.error |= L_BAD_ORDER;
            break;
        }
    }

    return list->error_inf.error;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_graph(list_t* list)
{
    assert(list);

    if ((list->error_inf.error & L_NULL_FICT) == L_NULL_FICT)
    {
        printf("Cannot create graph: NULL fict element\n");
        return -1;
    }

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

    // Create blocks with their owns colors
    list_node_t* elem = list->fict->next;
    for (size_t i = 1; i < list->size + 1; i++)
    {
        const char* color = {};
        if (elem != NULL)
        {
            if (elem->next != NULL || elem->prev != NULL)
            {
                if (elem == elem->next->prev)
                {
                    color = TRUE_EL;
                }

                else
                {
                    color = NOT_TRUE_EL;
                }
            }

            else
            {
                color = NULL_EL;
            }

            fprintf(dot_file,
                        "block_%p [label=\"INDEX=%p|DATA=" FORMAT_DATA "|NEXT=%p|PREV=%p\", fillcolor=\"%s\"];\n",
                        elem, elem, elem->data, elem->next, elem->prev, color);
        }

        else
        {
            color = ALL_NULL_EL;
            fprintf(dot_file,
                    "block_%p [label=\"INDEX=%p|DATA=---|NEXT=---|PREV=---\", fillcolor=\"%s\"];\n",
                    elem, elem, color);
        }
        elem = elem->next;
    }

    // Create block for fict element
    if (list->fict == NULL)
    {
        fprintf(dot_file,
                "block_%p [label=\"INDEX=%p|DATA=---|NEXT=---|PREV=---\", fillcolor=\"" ALL_NULL_EL "\"];\n",
                list->fict, list->fict);
    }

    else if (list->fict->next == NULL || list->fict->prev == NULL)
    {
        fprintf(dot_file,
                "block_%p [label=\"INDEX=%p|DATA=" FORMAT_DATA "|NEXT=%p|PREV=%p\", fillcolor=\"" NULL_EL "\"];\n",
                list->fict, list->fict, list->fict->data, list->fict->next, list->fict->prev);
    }

    else
    {
        fprintf(dot_file,
                "block_%p [label=\"INDEX=%p|DATA=" FORMAT_DATA "|NEXT=%p|PREV=%p\", fillcolor=\"" FICT_EL "\"];\n",
                list->fict, list->fict ,list->fict->data, list->fict->next, list->fict->prev);
    }

    // Create block for NULL address
    fprintf(dot_file,
            "NULL [label=\"NULL ADDRESS\", shape=octagon, fillcolor=\"#ff1212ff\"];\n");


    // for (size_t i = 0; i < list->size; i++)
    // {
    //     fprintf(dot_file, "block_%p -> block_%p [style=invis, weight=100];\n", i, i + 1);
    // }

    elem = list->fict;
    for (size_t i = 0; i < list->size; i++, elem = elem->next)
    {
        if (elem == NULL)
        {
            break;
        }

        if (elem->next == NULL)
        {
            fprintf(dot_file,
                    "block_%p -> NULL [color=\"#ff1212ff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                    elem);
            continue;
        }

        if (elem->prev == NULL)
        {
            fprintf(dot_file,
                    "NULL -> block_%p [color=\"#ff1212ff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                    elem);
            continue;
        }

        if (elem == elem->next->prev)
        {
            // if (elem->next == list->fict)
            // {
            //     fprintf(dot_file,
            //             "block_%p -> block0 [color=\"#4512ffff\", penwidth=1.5, arrowsize=0.6, constraint=true, dir=both];\n",
            //             i);
            // }
            // else
            // {
            //     fprintf(dot_file,
            //             "block_%p -> block_%p [color=\"#4512ffff\", penwidth=1.5, arrowsize=0.6, constraint=true, dir=both];\n",
            //             i, i + 1);
            // }
            fprintf(dot_file,
                    "block_%p -> block_%p [color=\"#4512ffff\", penwidth=1.5, arrowsize=0.6, constraint=true, dir=both];\n",
                    elem, elem->next);
        }

        else
        {
            fprintf(dot_file,
            "block_%p -> block_%p [color=\"#ff1212ff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
            elem, elem->next);
        }
    }

    fprintf(dot_file,
            "block_%p -> block_%p [color=\"#ff1414ff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
            list->fict, list->fict->prev);

    fprintf(dot_file, "}\n");
    fclose_file(dot_file);

    char buffer_name_file[100] = {};
    sprintf(buffer_name_file, "%s/img%d.png", ADDRESS_IMAGES, AMOUNT_IMAGES);

    char system_cmd[100] = {};
    sprintf(system_cmd, "dot %s -T png -o %s", NAME_DOT_FILE, buffer_name_file);
    int trash = system(system_cmd);
    (void) trash;

    AMOUNT_IMAGES++;

    return L_NOT_ERRORS;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int list_dump_html(list_t* list,
                   const char* reason)
{
    assert(list);
    FILE* html_file = fopen_file(NAME_HTML_FILE, "a");

    fprintf(html_file, "-----------------------------------------------------------------------------------\n");

    fprintf(html_file,
            BLUE_COLOR "Dump has called function \"%s\" in \"%s:%d\"</mark>\n",
            list->error_inf.name_func,
            list->error_inf.name_file,
            list->error_inf.number_line);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long current_time = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000) % TIME_DIV;
    fprintf(html_file,
            BLUE_COLOR "Time after start: [%lld] miliseconds</mark>\n",
            current_time - list->create_inf.time_create);

    fprintf(html_file, BLUE_COLOR "Reason to dump: %s</mark>\n", reason);

    if (list->error_inf.error == L_NOT_ERRORS)
    {
        fprintf(html_file, GREEN_COLOR "Not errors in list</mark>\n");
    }
    else
    {
        fprintf(html_file, RED_COLOR "Errors with list:</mark>\n");

        size_t counter = 1;
        int error_func = list->error_inf.error;
        // printf("ERROR: %d\n", error_func);

        for (int error = 1; error <= MAX_ERROR; error *= 2)
        {
            if ((error_func & error) == error)
            {
                fprintf(html_file, RED_COLOR "    %s</mark>\n", DESC_ERROR[counter]);
            }
            counter++;
        }
    }

    fprintf(html_file,
            BLUE_COLOR "List stats\n{\n    "
            "Name list: \"%s\"\n    "
            "Place creation: \"%s:%d\"\n    "
            "Size: [%zu]\n    "
            "Fict: [%p]\n}</mark>\n",
            list->create_inf.name_list,
            list->create_inf.name_file,
            list->create_inf.number_line,
            list->size,
            list->fict);

    if ((list->error_inf.error & L_NULL_FICT) != L_NULL_FICT)
    {
        fprintf(html_file,
                PURPLE_COLOR "ADDRESS         </mark> | "
                GREY_COLOR "DATA      </mark>"
                GREEN_COLOR "NEXT            </mark>"
                YELLOW_COLOR "PREV            </mark> \n");

        list_node_t* index = list->fict;
        for (size_t i = 0; i < list->size + 1; i++)
        {
            if (index->next == list->fict && i != list->size && i != 0)
            {
                for (size_t idx = i; idx < list->size + 1; idx++)
                {
                    fprintf(html_file, RED_COLOR "-------------------------------------------------------------</mark>\n");
                }
                break;
            }

            fprintf(html_file,
                    PURPLE_COLOR "[%p]</mark> | "
                    GREY_COLOR "[" DUMP_FORMAT_DATA "]</mark>"
                    GREEN_COLOR "[%p]</mark>"
                    YELLOW_COLOR "[%p]</mark>\n",
                    index, index->data, index->next, index->prev);

            index = index->next;
        }
    }

    // fprintf(html_file, "-----------------------------------------------------------------------------------\n");

    create_graph(list);

    char address[100] = {};
    sprintf(address, "%simg%d.png", ADDRESS_IMG_HTML, AMOUNT_IMAGES - 1);
    // printf("ADDRESS: %s\n", address);
    fprintf(html_file, "<img src=%s width=1000px>\n", address);

    fclose_file(html_file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------
// TODO Replace int index to pointers and check address
// -------------------------------------------------------------------------------------------------------
int list_dump(list_t* list,
              const char* reason)
{
    assert(list);

    list_dump_html(list, reason);

    return 0;
}
// -------------------------------------------------------------------------------------------------------


// Some support fuctions for dump
// -------------------------------------------------------------------------------------------------------
int create_head_html_file(void)
{
    FILE* html_file = fopen_file(NAME_HTML_FILE, "w");

    fprintf(html_file, "<pre style=\"font-family: 'Courier New', monospace; "
            "font-size: 14px; color: #e0e0e0; background-color: #1e1e1e; padding: 10px; border-radius: 6px;\">\n\n");
    fclose_file(html_file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int clean_dir_images(void)
{
    char address[200] = {};
    sprintf(address, "%s/trash.txt", ADDRESS_IMAGES);

    FILE* buffer = fopen_file(address, "w");
    fprintf(buffer, "This file delete error of system comand\n");
    fclose_file(buffer);

    char comand[200] = {};
    sprintf(comand, "rm -r %s/*", ADDRESS_IMAGES);
    int ret_value = system(comand);

    return ret_value;
}
// -------------------------------------------------------------------------------------------------------


// Support fuctions for default users
// -------------------------------------------------------------------------------------------------------
list_node_t* list_head(list_t* list)
{
    assert(list);

    return list->fict;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_node_t* list_next(list_node_t* index)
{
    if (index == NULL)
    {
        return NULL;
    }

    return index->next;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_node_t* list_prev(list_node_t* index)
{
    if (index == NULL)
    {
        return NULL;
    }

    return index->prev;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
lsd_t list_data(list_node_t* index)
{
    if (index == NULL)
    {
        return -1;
    }

    return index->data;
}
// -------------------------------------------------------------------------------------------------------

