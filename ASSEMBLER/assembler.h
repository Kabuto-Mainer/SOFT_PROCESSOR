#ifndef COMPILER_H
#define COMPILER_H

#include "../COMMON/const.h"

#define PRINT_TO_TEXT_FILE ON

#if PRINT_TO_TEXT_FILE == ON
#define PRINT_TEXT(text_stream, INT_CMD) fprintf(text_stream, "%d\n", INT_CMD);

#else
#define PRINT_TEXT(text_stream, INT_CMD) (void(0))
#endif


// TODO добавить ENUM для каждой ошибки и тогда text_error будет не нужен

#define EXIT_FUNCTION(name_file, amount_cmd, bin_code, text_str, text_error, return_error) \
printf("%s:%d: ERROR: %s\n", name_file, amount_cmd + 1, text_error); \
free(bin_code); \
free(text_str); \
return return_error;



// printf("%s:%d: ", name_input_file, amount_cmd + 1);
//                 printf("ERROR: invalid syntax\n");
//
//                 free(bin_code);
//                 free(text_str);
//                 return -1;



int my_assembler(const char* name_input_file, const char* name_asm_file, const char* name_text_file);

size_t pass_space(const char* string_place, size_t index);
size_t find_char (const char* string, const char symbol);


#endif // COMPILER_H
