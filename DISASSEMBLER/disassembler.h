#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../COMMON/const.h"

// Пока не вижу смысла записывать ошибки в биты
// Т.к. при ошибке на данный момент компилятор прекращает дальнейшие проверки


#define EXIT_FUNCTION(name_file, amount_cmd, bin_code, text_str, return_error) \
printf("%s:%d: ERROR: %s\n", name_file, amount_cmd + 1, DESCRIPTION_ERRORS[-1 * return_error - 1]); \
free(bin_code); \
free(text_str); \
return return_error;


int my_disassembler(const char* name_asm_file, const char* name_text_file);


#endif // DISASSEMBLER_H
