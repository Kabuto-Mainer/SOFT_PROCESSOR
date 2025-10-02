#include <stdio.h>
#include <assert.h>

#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/const.h"

#include "disassembler.h"



int main() {
    my_disassembler(NAME_TEXT_FILE, NAME_DISASMB_FILE);

    return 0;
}

int my_disassembler(const char* name_byte_file, const char* name_text_file) {
    assert(name_byte_file);
    assert(name_text_file);

    FILE* asm_stream = fopen_file(name_byte_file, "rb");
    FILE* text_stream = fopen_file(name_text_file, "w");

    char* byte_code = create_char_buffer(file_size_check(name_byte_file) + 1);
    if (byte_code == NULL) {
        return -1;
    }

    size_t amount_symbols = fread(byte_code, sizeof(char), file_size_check(name_byte_file), asm_stream);
    byte_code[amount_symbols] = '\0';
    // printf("ALL: \n%s", byte_code);
    size_t current_symbol = 0;

    fclose_file(asm_stream);

    while (current_symbol < amount_symbols) {
        size_t add_index = find_char(byte_code + current_symbol, ' ');

        byte_code[current_symbol + add_index] = '\0';

        char* comand = byte_code + current_symbol;
        current_symbol += add_index + 1;

        int int_comand = atoi(comand);

        switch (int_comand) {
            case INT_PUSH: {
                add_index = find_char(byte_code + current_symbol, '\n');
                byte_code[current_symbol + add_index] = '\0';

                fprintf(text_stream, "%s %d\n", STR_MASS_COMANDS[int_comand], atoi(byte_code + current_symbol));
                current_symbol += add_index + 1;
                break;
            }

            default: {
                fprintf(text_stream, "%s\n", STR_MASS_COMANDS[int_comand]);
            }
        }
    }
    fclose_file(text_stream);
    free(byte_code);

    return 0;
}





