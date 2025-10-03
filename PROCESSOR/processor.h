#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack_define.h"

struct proc_struct{
    stack_struct stack;
    int* bin_code;
    int C_E;
    int registers[AMOUNT_REGISTERS];
};

int my_processor(const char* name_asm_file);


int* create_bin_buffer(const char* name_bin_file,
                int* amount_elements,
                int* size_stack);

int processor_stk(proc_struct* proc_str,
                  const char* name_bin_file,
                  int* amount_elements);

int processor_destruct(proc_struct* proc);

#endif // PROSECCOR_H
