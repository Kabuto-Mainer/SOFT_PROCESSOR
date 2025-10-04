#ifndef O_SUPPORT_H
#define O_SUPPORT_H

#include <stdlib.h>

#include "color.h"


#define asrt_to_file(_File_ret_value) {if (_File_ret_value == -1) { \
 printf("%sERROR with file %s %d\n%s", _R_, __FILE__, __LINE__, _N_); \
 exit(1); } \
}

#define asrt_to_file_f(_File_ret_value) {if (_File_ret_value == NULL) { \
 printf("%sERROR with file %s %d\n%s", _R_, __FILE__, __LINE__, _N_); \
 exit(1); } \
 }


FILE* fopen_file(const char* name_file, const char* type_open);

void fclose_file(FILE* address);

size_t file_size_check(const char* name_file);


int* create_int_buffer(const size_t start_size);
char* create_char_buffer(const size_t start_size);

size_t pass_space(const char* string_place, size_t index);
size_t find_char (const char* string, const char symbol);

int check_inf(FILE* bin_file);

int* realloc_buffer(int* address, const size_t max_size);


#endif // O_SUPPORT_H
