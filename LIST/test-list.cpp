#include <stdio.h>
#include <assert.h>

#include "list-func.h"
#include "list-config.h"

#include "../COMMON/color.h"



int dump_list(list_t list);

int main(void)
{
    printf("Testing list\n");

    list_t test_list = {};

    list_inf_t list_inf = {};
    list_inf.create_inf.name_list = NAME(test_list);
    list_inf.create_inf.name_file = __FILE__;
    list_inf.create_inf.number_line = __LINE__;
    list_ctr(&test_list, list_inf);
    printf("Creating completed succesfull\n");

    list_insert_after(&test_list, 0, 60);
//
//     COMMON_VERIFIER(&test_list, "main", 1);
    list_insert_after(&test_list, 1, 60);
    // DUMP_LIST(&test_list, 0, 0);
//     COMMON_VERIFIER(&test_list, "main", 1);
    // list_dump(&test_list, NAME_LOG_FILE);

    list_print(&test_list);
    list_dtr(&test_list);
    return 0;
}

