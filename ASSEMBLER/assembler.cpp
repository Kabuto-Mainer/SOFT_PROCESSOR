#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/const.h"

int main(void) {
    my_assembler(NAME_INPUT_FILE, NAME_BIN_FILE, NAME_TEXT_FILE);

    return 0;
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
        if (*(string + i) == symbol || *(string + i) == '\n') {
            return i;
        }
    }
    return 0;
}


int my_assembler(const char* name_input_file, const char* name_asm_file, const char* name_text_file) {
    assert(name_input_file);
    assert(name_asm_file);
    assert(name_text_file);

    // printf("Name input: %s\n", name_input_file);
    // printf("Name asm: %s\n", name_asm_file);
    // printf("Name text: %s\n", name_text_file);

    FILE* input_stream = fopen_file(name_input_file, "rb");

#if PRINT_TO_TEXT_FILE == ON
    FILE* text_stream = fopen_file(name_text_file, "w");
#endif

    char* text_str = create_char_buffer(file_size_check(name_input_file) + 1);
    if (text_str == NULL) {
        return -1;
    }

    size_t amount_symbols = fread(text_str, sizeof(char), file_size_check(name_input_file), input_stream);
    text_str[amount_symbols] = '\0';
    size_t current_symbol = 0;


    int push_counter = 0; // Текущее количество элементов в стеке
    int max_push = 0; // Максимальное количество элементов в стеке

    int amount_cmd = 0;
    int amount_div_result = 0;

    size_t max_size = START_AMOUNT_CMD; // Only >= current_size

    size_t current_size = 2; // Only ++

    // bin_code = <amount_cmd> <max_push> <bin_code[2]> ...
    int* bin_code = create_int_buffer(START_AMOUNT_CMD);


    while (current_symbol <= amount_symbols) {
        size_t add_index = find_char(text_str + current_symbol, ' ');
        if (add_index == 0) {
            EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, SYNTAX);
        }

        text_str[current_symbol + add_index] = '\0';

        char* comand = text_str + current_symbol;
        current_symbol += add_index + 1;
        // PUSH 30  ~~ ADD\nDIV
        // -----^    -------^

        int argument = 0;

        if (current_size == max_size - (2 * sizeof(int))) {
            bin_code = realloc_buffer(bin_code, (size_t) max_size);

            if (bin_code == NULL) {
                return -1;
            }

            max_size *= MOD_REALLOC;
        }

        if (max_push < push_counter) {
            max_push = push_counter;
        }
//--------------------------------------------------------------
        if (strcmp(comand, STR_MASS_COMANDS[INT_PUSH]) == 0) {
            add_index = find_char(text_str + current_symbol, '\n');
            if (add_index == 0) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, NO_ARG);
            }

            text_str[current_symbol + add_index] = '\0';
            argument = atoi(text_str + current_symbol);
            argument *= MODE_DECISION;

            current_symbol += add_index + 1;

            if (argument < MIN_MEAN || argument > MAX_MEAN) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, LIMIT);
            }

            bin_code[current_size++] = INT_PUSH;
            bin_code[current_size++] = argument;

#if PRINT_TO_TEXT_FILE == ON
            fprintf(text_stream, "%d %d\n", INT_PUSH, argument);
#endif
            push_counter++;
        }
//---------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_ADD]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, FEW_ADD);
            }

            bin_code[current_size++] = INT_ADD;

            PRINT_TEXT(text_stream, INT_ADD);
            push_counter--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_SUB]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, FEW_SUB);
            }

            bin_code[current_size++] = INT_SUB;

            PRINT_TEXT(text_stream, INT_SUB);
            push_counter--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_DIV]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, FEW_DIV);
            }

            bin_code[current_size++] = INT_DIV;

            PRINT_TEXT(text_stream, INT_DIV);
            push_counter--;
            amount_div_result--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_MUL]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, FEW_MUL);
            }

            bin_code[current_size++] = INT_MUL;

            PRINT_TEXT(text_stream, INT_MUL);
            push_counter--;
            amount_div_result++;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_OUT]) == 0) {
            if (push_counter < 1) {
                EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, FEW_OUT);
            }

            bin_code[current_size++] = INT_OUT;

            PRINT_TEXT(text_stream, INT_OUT);
            push_counter--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_HLT]) == 0) {
            bin_code[current_size++] = INT_HLT;

            amount_cmd++;

            PRINT_TEXT(text_stream, INT_HLT);
            break;
        }
//-------------------------------------------------------------------------------------
        else {
            EXIT_FUNCTION(name_input_file, amount_cmd, bin_code, text_str, UNKNOWN_CMD);
        }
        amount_cmd++;
    }
    // В надежде на то, что current_size поместится в <int>
    bin_code[0] = (int) current_size;
    bin_code[1] = max_push;

    fclose_file(input_stream);

    FILE* bin_file = fopen_file(name_asm_file, "wb");
    if (fwrite(bin_code, sizeof(int), current_size, bin_file) != current_size) {
        printf("ERROR: write bin_code to bin_file was failed\n");
        free(bin_code);
        return -1;
    }
    fclose_file(bin_file);

    free(bin_code);
    free(text_str);
    printf("Compilation was completed\n");
    return 0;
}
