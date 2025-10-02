#ifndef COMPILER_H
#define COMPILER_H

#include "../COMMON/const.h"

#define PRINT_TO_TEXT_FILE ON


int my_assembler(const char* name_input_file, const char* name_asm_file, const char* name_text_file);

size_t pass_space(const char* string_place, size_t index);
size_t find_char (const char* string, const char symbol);


#endif // COMPILER_H
