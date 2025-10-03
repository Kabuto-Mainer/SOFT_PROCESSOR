#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "support.h"
#include "const.h"

FILE* fopen_file(const char* name_file, const char* type_open) {
    assert(name_file);

    FILE* buffer = fopen(name_file, type_open);
    asrt_to_file_f(buffer);

    return buffer;
}


void fclose_file(FILE* address) {
    assert(address);

    asrt_to_file( fclose(address) );
}


size_t file_size_check(const char* name_file) {
    assert(name_file);

    struct stat file_stat = {}; //

    if (stat(name_file, &file_stat) == -1) {
        printf("ERROR with stat\n");
        return 0;
    }
//     Вроде как размер не должен быть отрицательным
    return (size_t) file_stat.st_size;
}


int* create_int_buffer(const size_t start_size) {
    int* buffer_address = (int*) calloc(start_size, sizeof(int));

    if (buffer_address == NULL) {
        printf("ERROR with calloc in create_int_buffer\n");
        return NULL;
    }

    return buffer_address;
}


char* create_char_buffer(const size_t start_size) {
    char* buffer_address = (char*) calloc(start_size, sizeof(char));

    if (buffer_address == NULL) {
        printf("ERROR with calloc in create_int_buffer\n");
        return NULL;
    }

    return buffer_address;
}


int* realloc_buffer(int* address, const size_t max_size) {
    int* buffer_address = (int*) realloc(address, max_size * MOD_REALLOC * sizeof(int));

    if (buffer_address == NULL) {
        printf("ERROR with realloc in realloc_buffer\n");
        return NULL;
    }

    return buffer_address;
}



size_t pass_space(const char* string_place, size_t index) {
    assert(string_place);

    while (string_place[index] == ' ') {
        index++;
    }
    return index;
}


size_t find_char (const char* string, const char symbol) {
    assert(string);

    for (size_t i = 0; ; i++) {
        if (*(string + i) == symbol || *(string + i) == '\n' || *(string + i) == '\0') {
            return i;
        }
    }
    return 0;
}
