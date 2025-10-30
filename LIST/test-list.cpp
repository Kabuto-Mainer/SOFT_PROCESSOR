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

    insertAfter(&test_list, 0, 10);
    insertAfter(&test_list, 1, 20);
    insertAfter(&test_list, 2, 30);
    // deleteAfter(&test_list, 5);
    insertAfter(&test_list, 4, 40);
    insertAfter(&test_list, 3, 41);
    insertAfter(&test_list, 3, 42);

    // test_list.index_inf[3].next = 1000;
    // test_list.index_inf[1].next = 0;
    // printList(&test_list);

    insertAfter(&test_list, 3, 43);
    // insertAfter(&test_list, 3, 47);

    // deleteAfter(&test_list, 1);
    // insertAfter(&test_list, 0, 50);
    // insertAfter(&test_list, 4, 70);




    list_dtr(&test_list);
    return 0;
}


