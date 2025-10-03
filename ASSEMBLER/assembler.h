#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../COMMON/const.h"

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
    FILE_ERR = -12
};

const int AMOUNT_ERRORS = 12;

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
    "error with writing to file"
};


const int AMOUNT_VARIABLES = 3;


#define EXIT_FUNCTION(name_file, amount_cmd, bin_code, text_str, return_error) \
printf("%s:%d: ERROR: %s\n", name_file, amount_cmd + 1, DESCRIPTION_ERRORS[-1 * return_error - 1]); \
free(bin_code); \
free(text_str); \
return return_error;


asm_error_t my_assembler(const char* name_asm_file,
                const char* name_byte_file,
                const char* name_text_file);


int check_realloc(int** bin_code, size_t* max_size, size_t current_size);
size_t skip_comment(const char* asm_code, const char last_char);


// Синтаксис KBC - язык
const char DEC_VAR = '$';
const char COMMENT_CHAR = '#';





#endif // DISASSEMBLER_H
