#include <stdio.h>

#include <time.h>
#include <sys/time.h>

#include "my-list.h"

#include "../SUPPORT/prog-bar.h"

const size_t MAIN_ITER = 100;
const size_t IN_ITER = 100;

// int main(void)
// {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     long long time_start = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);

//     printf("Start test speed of common list\n");
//     printf("TIME START: %lld\n", time_start);
//     printf("Program run:\n");
//     printf("---------------------------------\n");

//     for (size_t i = 0; i < MAIN_ITER; i++)
//     {
//         // printf("Testing [%3zu] iter: [----------------------------------------]", i);
//         list_t list = {};
//         listCreate(&list);

//         list_node_t* index = getHead(&list);
//         for (size_t iter = 0; iter < IN_ITER; iter++)
//         {
//             for (int xz_all = 0; xz_all < 1000; xz_all++)
//             {
//                 for (int xz = 0; xz < 100; xz++)
//                 {
//                     insertAfter(&list, index, 50);
//                 }

//                 for (int xz = 99; xz > -1; xz--)
//                 {
//                     deleteCurrent(&list, getNext(index));
//                 }
//             }

//             printf("\r[%3zu] <%s>", i, PROG_BAR[iter]);
//         }

//         listDestroy(&list);
//         putchar('\n');
//     }

//     gettimeofday(&tv, NULL);
//     long long time_end = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);

//     printf("Stop program\n");
//     printf("TIME END: %lld\n", time_end);
//     printf("Time of running: %lld\n", time_end - time_start);
//     printf("Amount comands: %zu\n", MAIN_ITER * IN_ITER * 200 * 1000);
//     printf("End of testing speed\n");

//     return 0;
// }



int main()
{
    list_t list = {};
    listCreate(&list);

    insertAfter(&list, getHead(&list), 10);
    insertAfter(&list, getHead(&list), 20);
    insertAfter(&list, getHead(&list), 30);
    list.fict->prev = (list_node_t*) 0x0123456;
    insertAfter(&list, getHead(&list), 40);
    deleteCurrent(&list, getNext(getHead(&list)));



    // list_insert_after(&list, list_head(&list), 1);

    listDestroy(&list);
    // printf("ENTER ----------------------------------------------\n");


    return 0;
}
