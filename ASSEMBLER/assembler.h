#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../COMMON/config.h"
#include "../COMMON/color.h"
#include "../COMMON/comand.h"
#include "../COMMON/cmd-hash.h"
#include "../PROCESSOR/stack.h"
#include "../PROCESSOR/stack_define.h"

//------------------------------------------------------------------------------------------------
struct label_t
{
    char name[20];
    int address;
};
//------------------------------------------------------------------------------------------------
struct variable_t
{
    char name[20];
    int value;
};
//------------------------------------------------------------------------------------------------
struct asm_struct
{
    const char* name_file;
    FILE* text_stream;
    int* bin_code;
    char* asm_code;

    label_t* table_label;
    variable_t* table_var;

    int amount_line;
    int cur_char;
    int cur_element;
    int hash_cmd;
};
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
#define EXIT_FUNCTION(data, return_error) \
 printf(_R_ "%s:%d: ERROR: %s\n" _N_, data->name_file, data->amount_line + 1, DESCRIPTION_ERRORS[return_error]); \
 return return_error;
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
enum asm_error_t{
    A_NOT_ERRORS  = 0,
    FEW_PUSH      = 1,
    NO_ARG        = 2,
    UNKNOWN_REG   = 3,
    NO_ADR        = 4,
    NULL_ADR      = 5,
    INVALID_POINT = 6,
    FILE_ERROR    = 7,
    NO_REG        = 8,
    LIMIT         = 9,
    UNKNOWN_CMD   = 10,
    ERROR         = 11,
    FEW_IF        = 12,
    UNCORRECT_IF  = 13,
    UNKNOWN_SIGN  = 14,
    FEW_ELSE      = 15,
    NO_VAR        = 16,
    NO_INT        = 17
};
//------------------------------------------------------------------------------------------------
const char DESCRIPTION_ERRORS[][40] =
{
    "no errors in code\n",
    "too few argument to push\n",
    "no register to use\n",
    "unknown register\n",
    "no address for jump\n",
    "null address in file\n",
    "not correct use point\n",
    "error with file\n",
    "no register to use\n",
    "too big or small argument\n",
    "unknown comand\n",
    "ERROR\n",
    "few arguments for if\n",
    "not correct use if\n",
    "unknown sign\n",
    "no if for else\n",
    "no variable to use\n",
    "argument is not int\n"
};
//------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------
asm_error_t my_assembler (const char* name_asm_file,
                          const char* name_byte_file,
                          const char* name_text_file,
                          label_t* table_label,
                          int number_start,
                          display_t* disp_set);
//------------------------------------------------------------------------------------------------
int asm_create(asm_struct* asm_data,
               const char* name_asm_file,
               const char* name_text_file,
               label_t* table_label,
               variable_t* table_var);
//------------------------------------------------------------------------------------------------
int check_arg(display_t* disp_set,
              const char** name_if,
              const char** name_of,
              int argv,
              char* args[]);
//------------------------------------------------------------------------------------------------
asm_error_t check_J_cmd(asm_struct* asm_data,
                        int cmd);
//------------------------------------------------------------------------------------------------
asm_error_t check_NUM_cmd(asm_struct* asm_data,
                          size_t cur_el_struct);
//------------------------------------------------------------------------------------------------
asm_error_t check_REG_cmd(asm_struct* asm_data,
                          size_t cur_el_struct);
//------------------------------------------------------------------------------------------------
asm_error_t asm_check_cmd(asm_struct* asm_data);
//------------------------------------------------------------------------------------------------
int skip_comment(asm_struct* asm_data,
                     char* comand,
                     int add_index);
//------------------------------------------------------------------------------------------------
int check_label(asm_struct* asm_data,
                  char* comand,
                  int number_start,
                  int add_index);
//------------------------------------------------------------------------------------------------
int check_var(asm_struct* asm_data);
//------------------------------------------------------------------------------------------------
int check_realloc(int** bin_code,
                  size_t* max_size,
                  size_t current_size);
//------------------------------------------------------------------------------------------------
int char_reg_to_int(const char* name_reg);
//------------------------------------------------------------------------------------------------
int sign_to_j_cmd(const char* string);
//------------------------------------------------------------------------------------------------
int bin_find(const int hash_cmd);
//------------------------------------------------------------------------------------------------
int bubble_sort(const int* mass_num, const int amount_el);
//------------------------------------------------------------------------------------------------










#endif // DISASSEMBLER_H
