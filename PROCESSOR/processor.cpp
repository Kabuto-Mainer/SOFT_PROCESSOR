#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "processor.h"
#include "stack.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"


int main(void) {
    my_processor(NAME_BIN_FILE);

    return 0;
}


int my_processor(const char* name_asm_file) {
    assert(name_asm_file);

    FILE* file_bin = fopen_file(name_asm_file, "rb");


    size_t amount_elements = 0;
    if (fread(&amount_elements, sizeof(int), 1, file_bin) != 1) {
        fclose_file(file_bin);
        printf("ERROR: read amount_elements not correct\n");
        return -1;
    }

    int size_stack = 0;
    if (fread(&size_stack, sizeof(int), 1, file_bin) != 1) {
        fclose_file(file_bin);
        printf("ERROR: read size_check not correct\n");
        return -1;
    }

    stack_struct stack = {};
    if (stack_stk(&stack, size_stack, __FILE__,  __LINE__, NAME_RETURN(stack)) != 0) {
        fclose_file(file_bin);
        printf("ERROR: creating stack was not completed\n");
        return -1;
    }

    int* bin_code = create_int_buffer(amount_elements);
    // int read_size = fread(bin_code, sizeof(int), amount_elements, file_bin);
    // printf("read_size: %d\n", read_size);
    // printf("amount_elements: %d\n", amount_elements);
    // return 0;
    if (fread(bin_code, sizeof(int), amount_elements, file_bin) != amount_elements - 2) {
        stack_destruct(&stack);
        free(bin_code);
        fclose_file(file_bin);
        printf("ERROR: with read bin_code\n");
        return -1;
    }

    fclose_file(file_bin);


    for (unsigned int current_element = 0; current_element < amount_elements - 2; current_element++) {
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
