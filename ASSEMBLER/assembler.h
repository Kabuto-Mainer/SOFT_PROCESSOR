#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../COMMON/config.h"
#include "../COMMON/color.h"
#include "../COMMON/comand.h"
#include "../COMMON/cmd-hash.h"
#include "../PROCESSOR/stack.h"
#include "../PROCESSOR/stack_define.h"

enum type_arg_t {
    NO = 0,
    NUM = 1,
    REG = 2,
    ADR = 3
};


struct label_t {
    char name[20];
    int address;
};

struct variable_t {
    char name[20];
    int value;
};

struct asm_func_t {
    INT_HASH hash_cmd;
    int amount_arg;
    type_arg_t type_arg;
    INT_CMD number;
};

const asm_func_t CMD_INF[] = {
    {HASH_PUSH, 1, NUM, INT_PUSH},
    {HASH_ADD, 0, NO, INT_ADD},
    {HASH_SUB, 0, NO, INT_SUB},
    {HASH_DIV, 0, NO, INT_DIV},
    {HASH_MUL, 0, NO, INT_MUL},
    {HASH_SQRT, 0, NO, INT_SQRT},
    {HASH_OUT, 0, NO, INT_OUT},
    {HASH_HLT, 0, NO, INT_HLT},
    {HASH_POPR, 1, REG, INT_POPR},
    {HASH_PUSHR, 1, REG, INT_PUSHR},
    {HASH_IN, 0, NO, INT_IN},
    {HASH_JMP, 1, ADR, INT_JMP},
    {HASH_JB, 1, ADR, INT_JB},
    {HASH_JBE, 1, ADR, INT_JBE},
    {HASH_JA, 1, ADR, INT_JA},
    {HASH_JAE, 1, ADR, INT_JAE},
    {HASH_JE, 1, ADR, INT_JE},
    {HASH_JNE, 1, ADR, INT_JNE},
    {HASH_HACK, 0, NO, INT_HACK},
    {HASH_CALL, 1, ADR, INT_CALL},
    {HASH_RET, 0, NO, INT_RET},
    {HASH_POPM, 1, REG, INT_POPM},
    {HASH_PUSHM, 1, REG, INT_PUSHM},
    {HASH_PAINT, 1, ADR, INT_PAINT},
    {HASH_COLOR, 1, ADR, INT_COLOR},
    {HASH_DRAW, 1, ADR, INT_DRAW}
};


struct asm_struct {
    const char* name_file;
    FILE* text_stream;
    int* bin_code;
    char* asm_code;

    label_t* table_label;
    variable_t* table_var;

// Препроцессор

    int amount_line;
    int cur_char;
    int cur_element;
    int hash_cmd;
};

#define EXIT_FUNCTION(data, return_error) \
 printf(_R_ "%s:%d: ERROR: %s\n" _N_, data->name_file, data->amount_line + 1, DESCRIPTION_ERRORS[return_error]); \
 return return_error;


enum asm_error_t{
    A_NOT_ERRORS = 0,
    FEW_PUSH = 1,
    NO_ARG = 2,
    UNKNOWN_REG = 3,
    NO_ADR = 4,
    NULL_ADR = 5,
    INVALID_POINT = 6,
    FILE_ERROR = 7,
    NO_REG = 8,
    LIMIT = 9,
    UNKNOWN_CMD = 10,
    ERROR = 11,
    FEW_IF = 12,
    UNCORRECT_IF = 13,
    UNKNOWN_SIGN = 14,
    FEW_ELSE = 15,
    NO_VAR = 16,
    NO_INT = 17
};

char DESCRIPTION_ERRORS[][40] = {
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



asm_error_t my_assembler (const char* name_asm_file,
                          const char* name_byte_file,
                          const char* name_text_file,
                          label_t* table_label);

asm_error_t check_J_cmd(asm_struct* asm_data, int cmd);

asm_error_t check_comand(asm_struct* asm_data);

int check_realloc(int** bin_code, size_t* max_size, size_t current_size);

int char_reg_to_int(const char* name_reg);

int sign_to_j_cmd(const char* string);



#define CONTINUE \
 amount_cmd++; \
 amount_line++; \
 current_symbol += add_index + 1; \
 continue;

#define CMD_WITH_REG(INT_CMD) \
    char char_reg[4] = ""; \
    char trash[100] = ""; \
    \
    if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %s", trash, char_reg) == 1) { \
        EXIT_FUNCTION(asm_data, NO_REG); \
    } \
    \
    int reg = 0; \
    if ((reg = char_reg_to_int(char_reg)) == -1) { \
        EXIT_FUNCTION(asm_data, UNKNOWN_REG); \
    } \
    \
    asm_data->bin_code[asm_data->cur_element++] = INT_CMD; \
    asm_data->bin_code[asm_data->cur_element++] = reg; \
    \
    fprintf(asm_data->text_stream, "%08X %08X |", (unsigned int) INT_CMD, (unsigned int) reg); \
    break;







#endif // DISASSEMBLER_H
