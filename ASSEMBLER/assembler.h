#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../COMMON/const.h"
#include "../COMMON/color.h"

#define PRINT_TO_TEXT_FILE ON

#if PRINT_TO_TEXT_FILE == ON
#define PRINT_TEXT(text_stream, INT_CMD) fprintf(text_stream, "%d\n", INT_CMD);

#else
#define PRINT_TEXT(text_stream, INT_CMD) (void(0))
#endif

// Пока не вижу смысла записывать ошибки в биты
// Т.к. при ошибке на данный момент компилятор прекращает дальнейшие проверки
enum asm_error_t{
    NOT_ERRORS = 0,
    SYNTAX = -1,
    NO_ARG = -2,
    LIMIT = -3,
    FEW_ADD = -4,
    FEW_SUB = -5,
    FEW_DIV = -6,
    FEW_MUL = -7,
    FEW_OUT = -8,
    FEW_SQRT = -9,
    UNKNOWN_CMD = -10,
    NULL_ADR = -11,
    FILE_ERR = -12,
    FEW_REG = -13,
    ERR_PUSHR = -14,
    INVALID_REG = -15,
    NO_VARIABLE = -16,
    FEW_LOC = -17,
    INVALID_LOC = -18
};

const int AMOUNT_ERRORS = 18;

char DESCRIPTION_ERRORS[AMOUNT_ERRORS][40] = {
    "syntax is not valid\n",
    "no argument to push\n",
    "too big or little argument to push\n",
    "too few argument to ADD\n",
    "too few argument to SUB\n",
    "too few argument to DIV\n",
    "too few argument to MUL\n",
    "too few argument to OUT\n",
    "too few argument to SQRTn",
    "unknown command\n",
    "null address\n",
    "error with writing to file",
    "no register\n",
    "no variable ib register\n",
    "invalid register\n",
    "no variable in register\n",
    "no location to jump\n",
    "invalid location to jump\n"
};


#define EXIT_FUNCTION(name_file, amount_cmd, bin_code, text_str, return_error) \
 printf(_R_ "%s:%d: ERROR: %s\n" _N_, name_file, amount_cmd + 1, DESCRIPTION_ERRORS[-1 * return_error - 1]); \
 free(bin_code); \
 free(text_str); \
 return return_error;


asm_error_t my_assembler(const char* name_asm_file,
                const char* name_byte_file,
                const char* name_text_file);


int check_realloc(int** bin_code, size_t* max_size, size_t current_size);


int char_reg_to_int(const char* name_reg);






#endif // DISASSEMBLER_H
