#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include "LIB/my-list.h"

#include "../COMMON/support.h"
#include "../COMMON/color.h"


// -------------------------------------------------------------------------------------------------------
static int AMOUNT_IMAGES = 0;
// -------------------------------------------------------------------------------------------------------

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

    fprintf(html_file, "<pre style=\"font-family: 'Courier New', monospace; "
            "font-size: 14px; color: #e0e0e0; background-color: #1e1e1e; padding: 10px; border-radius: 6px;\">\n\n");
    fclose_file(html_file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int clean_dir_images(void)
{
    char comand[200] = {};
    sprintf(comand, "rm -r %s/*", ADDRESS_IMAGES_DIR);
    int ret_value = system(comand);

    return ret_value;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
list_error_t create_graph(list_t* list)
{
    assert(list);

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

    lsi_t* mass_index_inf = (lsi_t*) calloc(list->list_inf.capacity, sizeof(lsi_t));
    lsi_t* mass_index_free = (lsi_t*) calloc(list->list_inf.capacity, sizeof(lsi_t));

    if (mass_index_inf == NULL || mass_index_free == NULL)
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

        if (list->index_inf[index].prev != -1)
        {
            break;
        }

        mass_index_free[count_el_free++] = index;
        index = list->index_inf[index].next;
    }

    create_block(list,
                 mass_index_inf,
                 count_el_inf,
                 mass_index_free,
                 count_el_free,
                 dot_file);

    for (size_t i = 0; i < list->list_inf.capacity - 1; i++)
    {
        fprintf(dot_file, "block%zu -> block%zu [style=invis, weight=100];\n", i, i + 1);
    }

    fprintf(dot_file, "\n");
    create_lines(list,
                 mass_index_inf,
                 count_el_inf,
                 mass_index_free,
                 count_el_free,
                 dot_file);

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
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_lines(list_t* list,
                 lsi_t* mass_index,
                 size_t amount_index,
                 lsi_t* mass_free,
                 size_t amount_free,
                 FILE* dot_file)
{
    assert(list);
    assert(mass_index);
    assert(mass_free);
    assert(dot_file);

    for (size_t i = 0; i < list->list_inf.capacity; i++)
    {
        int flag = 0;
        for (size_t idx = 0; idx < amount_index; idx++)
        {
            if ((int) i == mass_index[idx])
            {
                fprintf(dot_file,
                        "block%zu -> block" FORMAT_LSI_T " [color=\"#382dd1ff\", penwidth=1.5, arrowsize=0.6, constraint=true, dir = both];\n",
                        i, list->index_inf[i].next);
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            for (size_t idx = 0; idx < amount_free; idx++)
            {
                if ((int) i == mass_free[idx])
                {
                    fprintf(dot_file,
                            "block%zu -> block" FORMAT_LSI_T " [color=\"#1dad10ff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                            i, list->index_inf[i].next);
                    flag = 1;
                    break;
                }
            }
        }
// TODO   Create new fucntions for create lines
        if (flag == 0)
        {
            if (list->index_inf[i].prev >= (int) list->list_inf.capacity)
            {
                fprintf(dot_file,
                        "block" FORMAT_LSI_T " [label=\"" FORMAT_LSI_T "\", shape=octagon, fillcolor=\"#d31414ff\"];\n",
                        list->index_inf[i].prev, list->index_inf[i].prev);
                fprintf(dot_file,
                        "block" FORMAT_LSI_T " -> block%zu [color=\"#ff0505ff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                        list->index_inf[i].prev, i);
                flag = 1;
            }

            if (list->index_inf[i].next >= (int) list->list_inf.capacity)
            {
                fprintf(dot_file,
                        "block" FORMAT_LSI_T " [label=\"" FORMAT_LSI_T "\", shape=octagon, fillcolor=\"#d31414ff\"];\n",
                        list->index_inf[i].next, list->index_inf[i].next);
                fprintf(dot_file,
                        "block%zu -> block" FORMAT_LSI_T " [color=\"#ff0a0aff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                        i, list->index_inf[i].next);
                flag = 1;
            }
        }

        if (flag == 0 && i != 0)
        {
            fprintf(dot_file,
                        "block%zu -> block" FORMAT_LSI_T " [color=\"#ff0a0aff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                        i, list->index_inf[i].next);
            fprintf(dot_file,
                        "block" FORMAT_LSI_T " -> block%zu [color=\"#ff0a0aff\", penwidth=1.5, arrowsize=0.6, constraint=true];\n",
                        list->index_inf[i].next, i);
        }
    }
    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_block(list_t* list,
                 lsi_t* mass_index,
                 size_t amount_index,
                 lsi_t* mass_free,
                 size_t amount_free,
                 FILE* dot_file)
{
    assert(list);
    assert(mass_index);
    assert(mass_free);
    assert(dot_file);


    for (size_t i = 0; i < list->list_inf.capacity; i++)
    {
        const char* color = "";

        if ((int) i == 0)
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
            for (size_t idx = 0; idx < amount_index; idx++)
            {
                if ((int) i == mass_index[idx])
                {
                    color = "#2ab8dbff";
                    break;
                }
            }
        }

        if (color[0] == '\0')
        {
            for (size_t idx = 0; idx < amount_free; idx++)
            {
                if ((int) i == mass_free[idx])
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

    fprintf(dot_file,
            "head [label=\"HEAD\", fillcolor=\"#636363ff\"];\n");
    fprintf(dot_file,
            "tail [label=\"TAIL\", fillcolor=\"#636363ff\"];\n");
    fprintf(dot_file,
            "free [label=\"FREE\", fillcolor=\"#11a03cff\"];\n");
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

    fprintf(html_file, "-----------------------------------------------------------------------------------\n");

    fprintf(html_file,
            BLUE_COLOR "Dump has called function \"%s\" in \"%s:%d\"</mark>\n",
            list->list_inf.error_inf.name_call_func,
            list->list_inf.error_inf.name_call_file,
            list->list_inf.error_inf.number_call_line);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long current_time = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000) % TIME_DIV;
    fprintf(html_file,
            BLUE_COLOR "Time after start: [%lld] miliseconds</mark>\n",
            current_time - list->list_inf.create_inf.time_start);

    fprintf(html_file, BLUE_COLOR "Reason to dump: %s</mark>", dump_reason);

    if (list->list_inf.error_inf.current_error == L_NOT_ERRORS)
    {
        fprintf(html_file, GREEN_COLOR "Not errors in list</mark>\n");
    }
    else
    {
        fprintf(html_file, RED_COLOR "Errors with list:</mark>\n");

        size_t counter = 1;
        int error_func = list->list_inf.error_inf.current_error;
        // printf("ERROR: %d\n", error_func);

        for (int error = 1; error <= MAX_ERROR; error *= 2)
        {
            if ((error_func & error) == error)
            {
                fprintf(html_file, RED_COLOR "    %s</mark>\n", DISC_ERROR[counter]);
            }
            counter++;
        }
    }

    fprintf(html_file, BLUE_COLOR "List stats:\n    Name list: \"%s\"\n    Place creation \"%s:%d\"\n    ----------------\n",
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
                PURPLE_COLOR "INDEX </mark> | "
                GREY_COLOR "DATA      </mark>"
                RED_COLOR " NEXT      </mark>"
                YELLOW_COLOR " PREV      </mark> \n");
        for (size_t i = 0; i < list->list_inf.capacity; i++)
        {
            fprintf(html_file,
                    PURPLE_COLOR "[%4zu]</mark> | "
                    GREY_COLOR "[" DUMP_FORMAT_LSD_T "]</mark>"
                    RED_COLOR " [" DUMP_FORMAT_LSI_T "]</mark>"
                    YELLOW_COLOR " [" DUMP_FORMAT_LSI_T "]</mark>\n",
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


