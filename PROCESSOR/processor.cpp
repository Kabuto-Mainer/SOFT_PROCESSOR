#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "processor.h"
#include "stack.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"


int main(int argv, char* args[]) {
    if (argv > 1) {
        FILE* buffer = fopen_file(args[1], "wb");
        fclose_file(buffer);

        printf("Using %s file\n", args[1]);
        my_processor(args[1]);
    }

    else {
        my_processor(NAME_BIN_FILE);
    }

    return 0;
}

int* create_bin_buffer(const char* name_bin_file,
                int* amount_elements,
                int* size_stack) {
    assert(name_bin_file);
    assert(size_stack);
    assert(amount_elements);

    FILE* bin_file = fopen_file(name_bin_file, "rb");

    if (fread(amount_elements, sizeof(int), 1, bin_file) != 1) {
        fclose_file(bin_file);
        printf("ERROR: read amount_elements not correct\n");
        return NULL;
    }

    if (fread(size_stack, sizeof(int), 1, bin_file) != 1) {
        fclose_file(bin_file);
        printf("ERROR: read size_check not correct\n");
        return NULL;
    }

//! Нам точно известно, что amount_elements > 2
    int* bin_code = create_int_buffer(size_t (*amount_elements - 2));
    if (fread(bin_code, sizeof(int), size_t(*amount_elements - 2), bin_file) != size_t(*amount_elements - 2)) {
        free(bin_code);
        fclose_file(bin_file);
        printf("ERROR: with read bin_code\n");
        return NULL;
    }

    fclose_file(bin_file);
    return bin_code;
}


int my_processor(const char* name_bin_file) {
    assert(name_bin_file);

    int amount_elements = 0;
    int size_stack = 0;

    int* bin_code = create_bin_buffer(name_bin_file, &amount_elements, &size_stack);
    if (bin_code == NULL) {
        return -1;
    }

    stack_struct stack = {};
    if (stack_stk(&stack, size_stack, __FILE__,  __LINE__, NAME_RETURN(stack)) != 0) {
        printf("ERROR: creating stack was not completed\n");
        return -1;
    }

    for (unsigned int current_element = 0; current_element <size_t( amount_elements - 2); current_element++) {
        switch (bin_code[current_element]) {
            case INT_PUSH: {
                current_element++;

                stack_push(&stack, bin_code[current_element]);
                break;
            }

            case INT_ADD: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&stack, &arg_1);
                stack_pop(&stack, &arg_2);

                stack_push(&stack, arg_1 + arg_2);
                break;
            }

            case INT_SUB: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&stack, &arg_1);
                stack_pop(&stack, &arg_2);

                stack_push(&stack, arg_1 - arg_2);
                break;
            }

            case INT_DIV: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&stack, &arg_1);
                stack_pop(&stack, &arg_2);

                if (arg_2 == 0) {
                    printf("ERROR: division by zero is not correct\n");
                    stack_destruct(&stack);
                    free(bin_code);
                    return -1;
                }

                stack_push(&stack, arg_1 / arg_2);
                break;
            }

            case INT_MUL: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&stack, &arg_1);
                stack_pop(&stack, &arg_2);

                stack_push(&stack, arg_1 * arg_2);
                break;
            }

            case INT_OUT: {
                int arg = 0;

                stack_pop(&stack, &arg);
                printf("%d\n", arg);
                break;
            }

            case INT_HLT: {
                stack_destruct(&stack);
                free(bin_code);

                return 0;
            }

            default: {
                stack_destruct(&stack);
                free(bin_code);

                printf("ERROR: unknown command\n");
                return -1;
            }
        }
    }
    stack_destruct(&stack);
    free(bin_code);
    return -1;
}
