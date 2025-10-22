#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "asm-type.h"
#include "asm-sort.h"
#include "../COMMON/comand.h"
#include "../PROCESSOR/proc-type.h"



//------------------------------------------------------------------------------------------------
// TODO +-
int sort_comp_label(const void* struct_1, const void* struct_2)
{
    const label_t* label_1 = (const label_t*) struct_1;
    const label_t* label_2 = (const label_t*) struct_2;

    return (label_1->hash_label - label_2->hash_label);
}
//------------------------------------------------------------------------------------------------
int search_comp_label(const void* hash_label, const void* label_struct)
{
    const int* hash = (const int*) hash_label;
    const label_t* label_data = (const label_t*) label_struct;

    return (*hash - label_data->hash_label);
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int sort_comp_var(const void* struct_1, const void* struct_2)
{
    const variable_t* var_1 = (const variable_t*) struct_1;
    const variable_t* var_2 = (const variable_t*) struct_2;

    return (var_1->hash_var - var_2->hash_var);
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int search_comp_var(const void* hash_var, const void* var_struct)
{
    const int* hash = (const int*) hash_var;
    const variable_t* var_data = (const variable_t*) var_struct;

    return (*hash - var_data->hash_var);
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int compare_asm_func(const void* struct_1, const void* struct_2)
{
    const asm_func_t* func_1 = (const asm_func_t*) struct_1;
    const asm_func_t* func_2 = (const asm_func_t*) struct_2;

    return (func_1->hash_cmd - func_2->hash_cmd);
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int sort_label(asm_struct* asm_data)
{
    assert(asm_data);

    qsort(asm_data->table_label,
          (size_t) asm_data->amount_labels,
          sizeof(asm_data->table_label[0]),
          &sort_comp_label);

    return 0;
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int sort_var(asm_struct* asm_data)
{
    assert(asm_data);

    qsort(asm_data->table_var,
          (size_t) asm_data->amount_vars,
          sizeof(asm_data->table_var[0]),
          &sort_comp_var);

    return 0;
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int search_label(asm_struct* asm_data, int hash_label)
{
    assert(asm_data);

    int* return_value = (int*) bsearch(&hash_label,
                                       asm_data->table_label,
                                       (size_t) asm_data->amount_labels,
                                       sizeof(asm_data->table_label[0]),
                                       &search_comp_label);

    return *return_value;
}
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
int search_var(asm_struct* asm_data, int hash_var)
{
    assert(asm_data);

    int* return_value = (int*) bsearch((void*) &hash_var,
                                        asm_data->table_var,
                                        (size_t) asm_data->amount_vars,
                                        sizeof(asm_data->table_var[0]),
                                        &search_comp_var);

    return *return_value;
}
//------------------------------------------------------------------------------------------------



