#include <stdio.h>

#include "my-list.h"


int main()
{
    list_t list = {};
    listCreate(&list);

    insertAfter(&list, getHead(&list), 10);
    insertAfter(&list, getHead(&list), 20);
    insertAfter(&list, getHead(&list), 30);
    insertAfter(&list, getHead(&list), 40);
    deleteCurrent(&list, getNext(getHead(&list)));



    // list_insert_after(&list, list_head(&list), 1);

    listDestroy(&list);
    // printf("ENTER ----------------------------------------------\n");


    return 0;
}
