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
#include "../COMMON/color.h"


int main(int argv, char* args[]) {

    if (argv > 1) {
        FILE* buffer = fopen_file(args[1], "wb");
        fclose_file(buffer);

        printf(_G_ "Using %s file\n" _N_, args[1]);
        my_proc(args[1]);
    }

    else {
        my_proc(NAME_BIN_FILE);
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

    if (check_inf(bin_file) != 0) {
        return NULL;
    }

    if (fread(amount_elements, sizeof(int), 1, bin_file) != 1) {
        fclose_file(bin_file);
        printf(_R_ "ERROR: read amount_elements not correct\n" _N_);
        return NULL;
    }

    if (fread(size_stack, sizeof(int), 1, bin_file) != 1) {
        fclose_file(bin_file);
        printf(_R_ "ERROR: read size_check not correct\n" _N_);
        return NULL;
    }

//! Нам точно известно, что amount_elements > 2
    int* bin_code = create_int_buffer(size_t (*amount_elements - AMOUNT_SUP_NUM));
    if (fread(bin_code, sizeof(int), size_t(*amount_elements - AMOUNT_SUP_NUM), bin_file)
        != size_t(*amount_elements - AMOUNT_SUP_NUM)) {

        free(bin_code);
        fclose_file(bin_file);
        printf(_R_ "ERROR: with read bin_code\n" _N_);
        return NULL;
    }

    fclose_file(bin_file);
    return bin_code;
}


int my_proc(const char* name_bin_file) {
    assert(name_bin_file);

    int amount_elements = 0;
    cpu_t proc = {};

    if (cpu_ctor(&proc, name_bin_file, &amount_elements) == -1) {
        printf(_R_ "ERROR: cpu_ctor return -1\n" _N_);
        return -1;
    }

    for ( ; size_t (proc.C_E) < size_t (amount_elements - AMOUNT_SUP_NUM); (proc.C_E)++) {
        // if (getchar() == 'd') {
        //     proc_dump(&proc);
        // }

        switch (proc.bin_code[proc.C_E]) {
            case INT_PUSH: {
                proc.C_E++;

                stack_push(&(proc.stack), proc.bin_code[proc.C_E]);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_IN: {
                int num = 0;

                printf(_G_ "Enter int num: " _N_);
                scanf("%d", &num);

                stack_push(&(proc.stack), num);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_ADD: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                stack_push(&(proc.stack), arg_1 + arg_2);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_SUB: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                stack_push(&(proc.stack), arg_1 - arg_2);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_DIV: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                if (arg_2 == 0) {
                    printf(_R_ "ERROR: division by zero is not correct\n" _N_);
                    cpu_dtor(&proc);
                    return -1;
                }

                stack_push(&(proc.stack), arg_1 / arg_2);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_MUL: {
                int arg_1 = 0;
                int arg_2 = 0;

                stack_pop(&(proc.stack), &arg_1);
                stack_pop(&(proc.stack), &arg_2);

                stack_push(&(proc.stack), arg_1 * arg_2);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_SQRT: {
                int arg = 0;

                stack_pop(&(proc.stack), &arg);
                stack_push(&(proc.stack), (int) sqrt(arg));
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_OUT: {
                int arg = 0;

                stack_pop(&(proc.stack), &arg);
                printf("%d\n", arg);
                break;
            }
//------------------------------------------------------------------------------------------------
//             case INT_CODE: {
//                 printf("%s\n", STR_CODS[proc.bin_code[++proc.C_E]]);
//
//                 break;
//             }
            case INT_HLT: {
                cpu_dtor(&proc);
                return 0;
            }
//------------------------------------------------------------------------------------------------
            case INT_POPR: {
                int reg = proc.bin_code[++proc.C_E];
                int num = 0;

                stack_pop(&(proc.stack), &num);
                proc.regs[reg] = num;
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_PUSHR: {
                int reg = proc.bin_code[++proc.C_E];
                int num = proc.regs[reg];

                stack_push(&(proc.stack), num);
                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_JMP: {
                proc.C_E++;
                proc.C_E = proc.bin_code[proc.C_E] - 2;

                break;
            }

            case INT_HACK: {
                // proc.stack.size = -1;
                int arg_1 = 100;

                proc.stack.data[2] = arg_1;

                break;
            }
//------------------------------------------------------------------------------------------------
            case INT_JB: {
                J_COMAND(<);
            }
//---------------------------------------------------------------------------------------------------
            case INT_JBE: {
                J_COMAND(<=);
            }
//---------------------------------------------------------------------------------------------------
            case INT_JA: {
                J_COMAND(>);
            }
//---------------------------------------------------------------------------------------------------
            case INT_JAE: {
                J_COMAND(>=);
            }
//---------------------------------------------------------------------------------------------------
            case INT_JE: {
                J_COMAND(==);
            }
//---------------------------------------------------------------------------------------------------
            case INT_JNE: {
                J_COMAND(!=);
            }
//----------------------------------------------------------------------------------------------------
            default: {
                cpu_dtor(&proc);
                printf(_R_ "ERROR: unknown command\n" _N_);
                return -1;
            }
        }
    }
    printf(_R_ "Invalid go???\n" _N_);
    cpu_dtor(&proc);
    return -1;
}


int cpu_ctor(cpu_t* proc,
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

//TODO убрать это тест
    size_stack = START_SIZE_STACK;

    if (stack_creator(&(proc->stack), size_stack, __FILE__,  __LINE__, NAME_RETURN(stack)) != 0) {
        printf(_R_ "ERROR: creating stack was not completed\n" _N_);
        return -1;
    }

    for (int i = 0; i < AMOUNT_REGISTERS; i++) {
        proc->regs[i] = 0;
    }

    proc->C_E = 0;
    proc->amount_el = *amount_elements;

    return 0;
}


int cpu_dtor(cpu_t* proc) {
    assert(proc);

    stack_destruct(&(proc->stack));
    free(proc->bin_code);

    return 0;
}


int proc_dump(cpu_t* proc) {
    assert(proc);

    // stack_dump(&(proc->stack)); // Выводит слишком много
    // printf(_R_ "\n=== STACK ===\n\n" _N_);

    printf(_P_"size = %s%zd%s\n", _B_, proc->stack.size, _P_);
    // printf("capacity = %s%zd%s\n", _B_, proc->stack.capacity, _P_);
    // printf("data %s[%p]%s\n" _N_ , _B_, proc->stack.data, _P_);

    print_stack_for_dump(&(proc->stack), NOT_ERRORS);

    printf(_R_ "\n=== BIN_CODE ===\n\n" _N_);

    print_before_end(proc);
    print_end(proc);

    printf(_R_ "\n=== Registers ===\n\n");

    print_reg(proc);

    return 0;
}


int print_before_end(cpu_t* proc) {
    assert(proc);

    for (unsigned int i = 0; i < ((unsigned) (proc->amount_el - AMOUNT_SUP_NUM) / 8); i++) {
        print_line(proc, i);
    }

    return 0;
}

int print_end(cpu_t* proc) {
    assert(proc);

    // print_line(proc, ((unsigned) (proc->amount_el - AMOUNT_SUP_NUM) % 8 ));
    printf(_G_ "[%p]: " _N_, proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8);

    for (int i = 0; i < (proc->amount_el - AMOUNT_SUP_NUM) % 8; i++) {
        if ((((proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8) + i) == proc->C_E ) {
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 0));
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 1));
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 2));
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 3));
        }

        else if (i % 2 == 0) {
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 0));
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 1));
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 2));
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 3));
        }
        else {
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 0));
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 1));
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 2));
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + (proc->amount_el - AMOUNT_SUP_NUM) / 8 * 8 + i) + 3));
        }
    }
    // for (unsigned i = 0; i < ((unsigned) (proc->amount_el - AMOUNT_SUP_NUM) % 8); i++) {
    //     print_line(proc, ((unsigned) (proc->amount_el - AMOUNT_SUP_NUM) / 8 - 1));
    // }

    return 0;
}


int print_line(cpu_t* proc, unsigned int i) {
    assert(proc);

    printf(_G_ "[%p]: " _N_, proc->bin_code + i * 8);

    for (unsigned int buf = 0; buf < 8; buf++) {
        if (i * 8 + buf == (unsigned) proc->C_E ) {
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 0));
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 1));
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 2));
            printf(_G_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 3));
        }

        else if (buf % 2 == 0) {
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 0));
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 1));
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 2));
            printf(_B_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 3));
        }

        else {
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 0));
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 1));
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 2));
            printf(_P_ "%02X " _N_, *((unsigned char*) (proc->bin_code + i * 8 + buf) + 3));
        }

    }
    putchar('\n');
    return 0;
}


int print_reg(cpu_t* proc) {
    assert(proc);

    for (int i = 0; i < AMOUNT_REGISTERS; i++) {
        printf(_P_ "%s: %s%08d\n" _N_, CHAR_REG[i], _B_, proc->regs[i]);
    }

    return 0;
}


