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

int my_proc(const char* name_asm_file);


int* create_bin_buffer(const char* name_bin_file,
                int* amount_elements,
                int* size_stack);

int proc_creator(cpu_t* proc_str,
                  const char* name_bin_file,
                  int* amount_elements);

int proc_destruct(cpu_t* proc);


int proc_dump(cpu_t* proc);

int print_before_end(cpu_t* proc);
int print_end(cpu_t* proc);

int print_current(cpu_t* proc);
int print_after_current(cpu_t* proc);
int print_line(cpu_t* proc, unsigned int i);



#endif // PROSECCOR_H
