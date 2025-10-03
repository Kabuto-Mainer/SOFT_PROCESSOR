#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <math.h>

#include "stack_define.h"
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

    int signature = 0;
    if (fread(&signature, sizeof(int), 1, bin_file) != 1) {
        fclose_file(bin_file);
        printf("ERROR: read signature not correct\n");
        return NULL;
    }

    if (signature != OWN_SIGNATURE) {
        printf("ERROR: signature is not coincidence\n");
        return NULL;
    }

    int version = 0;
    if (fread(&version, sizeof(int), 1, bin_file) != 1) {
        fclose_file(bin_file);
        printf("ERROR: read version not correct\n");
        return NULL;
    }

    if (version != CURRENT_VERSION) {
        printf("ERROR: invalid version\n");
        return NULL;
    }

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
    int* bin_code = create_int_buffer(size_t (*amount_elements - AMOUNT_SUP_NUM));
    if (fread(bin_code, sizeof(int), size_t(*amount_elements - AMOUNT_SUP_NUM), bin_file)
        != size_t(*amount_elements - AMOUNT_SUP_NUM)) {

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
    proc_struct proc = {};

    if (processor_stk(&proc, name_bin_file, &amount_elements) == -1) {
        printf("ERROR: processor_stk return -1\n");
        return -1;
    }

    for ( ; size_t (proc.C_E) < size_t (amount_elements - AMOUNT_SUP_NUM); (proc.C_E)++) {
        switch (proc.bin_code[proc.C_E]) {
            case INT_PUSH: {
                proc.C_E++;

                stack_push(&(proc.stack), proc.bin_code[proc.C_E]);
                break;
            }

            case INT_IN: {
                int num = 0;

                printf("Enter int num: ");
                scanf("%d", &num);

                stack_push(&(proc.stack), num);
                break;
            }

            case INT_ADD: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                stack_push(&(proc.stack), arg_1 + arg_2);
                break;
            }

            case INT_SUB: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                stack_push(&(proc.stack), arg_1 - arg_2);
                break;
            }

            case INT_DIV: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                if (arg_2 == 0) {
                    printf("ERROR: division by zero is not correct\n");
                    processor_destruct(&proc);
                    return -1;
                }

                stack_push(&(proc.stack), arg_1 / arg_2);
                break;
            }

            case INT_MUL: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                stack_push(&(proc.stack), arg_1 * arg_2);
                break;
            }

            case INT_SQRT: {
                int arg = 0;

                stack_pop(&(proc.stack), &arg);
                stack_push(&(proc.stack), (int) sqrt(arg));
                break;
            }

            case INT_OUT: {
                int arg = 0;

                stack_pop(&(proc.stack), &arg);
                printf("%d\n", arg);
                break;
            }

            case INT_HLT: {
                processor_destruct(&proc);
                return 0;
            }

            case INT_POPR: {
                int reg = proc.bin_code[++proc.C_E];
                int num = 0;

                stack_pop(&(proc.stack), &num);
                proc.registers[reg] = num;
                break;
            }

            case INT_PUSHR: {
                int reg = proc.bin_code[++proc.C_E];
                int num = proc.registers[reg];

                stack_push(&(proc.stack), num);
                break;
            }

            default: {
                processor_destruct(&proc);
                printf("ERROR: unknown command\n");
                return -1;
            }
        }
    }
    processor_destruct(&proc);
    return -1;
}


int processor_stk(proc_struct* proc,
                  const char* name_bin_file,
                  int* amount_elements) {
    assert(proc);
    assert(name_bin_file);

    int size_stack = 0;

    int* bin_code = create_bin_buffer(name_bin_file, amount_elements, &size_stack);
    if (bin_code == NULL) {
        return -1;
    }
    proc->bin_code = bin_code;

    if (stack_stk(&(proc->stack), size_stack, __FILE__,  __LINE__, NAME_RETURN(stack)) != 0) {
        printf("ERROR: creating stack was not completed\n");
        return -1;
    }

    for (int i = 0; i < AMOUNT_REGISTERS; i++) {
        proc->registers[i] = 0;
    }

    proc->C_E = 0;

    return 0;
}

int processor_destruct(proc_struct* proc) {
    assert(proc);

    stack_destruct(&(proc->stack));
    free(proc->bin_code);

    return 0;
}
