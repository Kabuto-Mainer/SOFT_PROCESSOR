#include <stdio.h>
#include <assert.h>

#include <time.h>
#include <sys/time.h>

#include "LIB/my-list.h"

#include "../SUPPORT/prog-bar.h"

int test(void);

const size_t MAIN_ITER = 100;
const size_t IN_ITER = 100;

// int main(void)
// {
//     list_t test_list = {};
//     listCreate(&test_list);

//     insertAfter(&test_list, 0, 50);
//     test_list.index_inf[1].next = 1000;
//     insertAfter(&test_list, 1, 40);
//     insertAfter(&test_list, 2, 30);
//     insertAfter(&test_list, 3, 20);

//     deleteCurrent(&test_list, 2);


//     list_dtr(&test_list);


//     return 0;
// }


int main(void)
{
    struct timeval tv = {};
    gettimeofday(&tv, NULL);
    long long time_start = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);

    printf("Start test speed of mode list\n");
    printf("TIME START: %lld\n", time_start);
    printf("Program run:\n");
    printf("---------------------------------\n");

    for (size_t i = 0; i < MAIN_ITER; i++)
    {
        // printf("Testing [%3zu] iter: [----------------------------------------]", i);
        list_t test_list = {};
        listCreate(&test_list);

        for (size_t iter = 0; iter < IN_ITER; iter++)
        {
            for (int xz_all = 0; xz_all < 1000; xz_all++)
            {
                for (int xz = 0; xz < 100; xz++)
                {
                    insertAfter(&test_list, xz, (int) xz * 100);
                }

                for (int xz = 99; xz > -1; xz--)
                {
                    deleteCurrent(&test_list, xz);
                }
            }

            printf("\r[%3zu]: [%s]", i, PROG_BAR[iter]);
        }
        list_dtr(&test_list);
        putchar('\n');
    }

    gettimeofday(&tv, NULL);
    long long time_end = ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);

    printf("Stop program\n");
    printf("TIME END: %lld\n", time_end);
    printf("Time of running: %lld\n", time_end - time_start);
    printf("Amount comands: %zu\n", MAIN_ITER * IN_ITER * 200 * 1000);
    printf("End of testing speed\n");

    return 0;
}


