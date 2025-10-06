#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack_define.h"
#include "../COMMON/color.h"

struct cpu_t{
    stack_struct stack;
    int* bin_code;
    int C_E;
    int regs[AMOUNT_REGISTERS];
    int amount_el;
};

#define J_COMAND(OP) \
    int arg_1 = 0; \
    int arg_2 = 0; \
    \
    stack_pop(&(proc.stack), &arg_1); \
    stack_pop(&(proc.stack), &arg_2); \
    \
    proc.C_E++; \
    if (arg_1 OP arg_2) { \
        proc.C_E = proc.bin_code[proc.C_E] - 2; \
    } \
    break;

int my_proc(const char* name_asm_file);


int* create_bin_buffer(const char* name_bin_file,
                int* amount_elements,
                int* size_stack);

int cpu_ctor(cpu_t* proc_str,
                  const char* name_bin_file,
                  int* amount_elements);

int cpu_dtor(cpu_t* proc);


int proc_dump(cpu_t* proc);

int print_before_end(cpu_t* proc);
int print_end(cpu_t* proc);

int print_reg(cpu_t* proc);
int print_line(cpu_t* proc, unsigned int i);



#endif // PROSECCOR_H
