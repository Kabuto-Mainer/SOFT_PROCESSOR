#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/const.h"

int main(void) {
    variable massive_variable[AMOUNT_VARIABLES] = {};
    completion_name_table(massive_variable);

    my_assembler(NAME_ASM_FILE, NAME_BIN_FILE, NAME_TEXT_FILE, massive_variable);

    return 0;
}


int my_assembler(const char* name_asm_file,
                const char* name_byte_file,
                const char* name_text_file,
                variable* name_table) {
    assert(name_asm_file);
    assert(name_byte_file);
    assert(name_text_file);

    // printf("Name input: %s\n", name_asm_file);
    // printf("Name asm: %s\n", name_byte_file);
    // printf("Name text: %s\n", name_text_file);

    FILE* asm_stream = fopen_file(name_asm_file, "rb");

#if PRINT_TO_TEXT_FILE == ON
    FILE* text_stream = fopen_file(name_text_file, "w");
#endif

    char* asm_code = create_char_buffer(file_size_check(name_asm_file) + 1);
    if (asm_code == NULL) {
        return -1;
    }

    size_t amount_symbols = fread(asm_code, sizeof(char), file_size_check(name_asm_file), asm_stream);
    asm_code[amount_symbols] = '\0';
    size_t current_symbol = 0;


    int push_counter = 0; // Текущее количество элементов в стеке
    int max_push = 0; // Максимальное количество элементов в стеке

    int amount_cmd = 0;
    int amount_div_result = 0;

    size_t max_elements = START_AMOUNT_CMD; // Only >= current_element

    size_t current_element = 2; // Only ++

    // bin_code = <amount_cmd> <max_push> <bin_code[2]> ...
    int* bin_code = create_int_buffer(START_AMOUNT_CMD);


    while (current_symbol <= amount_symbols) {
        size_t buffer_add = skip_comment(asm_code + current_symbol, '\n');

        if (buffer_add != 0) {
            current_symbol += buffer_add;
            amount_cmd--;
            continue;
        }

        size_t add_index = find_char(asm_code + current_symbol, ' ');
        if (add_index == 0) {
            EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, SYNTAX);
        }

        asm_code[current_symbol + add_index] = '\0';

        char* comand = asm_code + current_symbol;
        current_symbol += add_index + 1;
        // PUSH 30  ~~ ADD\nDIV
        // -----^    -------^

        int argument = 0;

// Проверки размеров
//-----------------------------------------------------------------
        if (check_realloc(&bin_code, &max_elements, current_element) == -1) {
            free(bin_code);
            free(asm_code);
            return -1;
        }

        if (max_push < push_counter) {
            max_push = push_counter;
        }
//--------------------------------------------------------------
        if (strcmp(comand, STR_MASS_COMANDS[INT_PUSH]) == 0) {
            add_index = find_char(asm_code + current_symbol, '\n');
            if (add_index == 0) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, NO_ARG);
            }

            asm_code[current_symbol + add_index] = '\0';
            if (*(asm_code + current_symbol) == DEC_VAR) {
                argument = find_mean_variable(asm_code + current_symbol, name_table);
            }

            else {
                argument = atoi(asm_code + current_symbol);
            }
            argument *= MODE_DECISION;

            current_symbol += add_index + 1;

            if (argument < MIN_MEAN || argument > MAX_MEAN) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, LIMIT);
            }

            bin_code[current_element++] = INT_PUSH;
            bin_code[current_element++] = argument;

#if PRINT_TO_TEXT_FILE == ON
            fprintf(text_stream, "%d %d\n", INT_PUSH, argument);
#endif
            push_counter++;
        }
//---------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_ADD]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_ADD);
            }

            bin_code[current_element++] = INT_ADD;

            PRINT_TEXT(text_stream, INT_ADD);
            push_counter--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_SUB]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_SUB);
            }

            bin_code[current_element++] = INT_SUB;

            PRINT_TEXT(text_stream, INT_SUB);
            push_counter--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_DIV]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_DIV);
            }

            bin_code[current_element++] = INT_DIV;

            PRINT_TEXT(text_stream, INT_DIV);
            push_counter--;
            amount_div_result--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_MUL]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_MUL);
            }

            bin_code[current_element++] = INT_MUL;

            PRINT_TEXT(text_stream, INT_MUL);
            push_counter--;
            amount_div_result++;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_SQRT]) == 0) {
            bin_code[current_element++] = INT_SQRT;
            if (push_counter < 1) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_SQRT);
            }
            bin_code[current_element++] = INT_SQRT;

            PRINT_TEXT(text_stream, INT_SQRT);
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_OUT]) == 0) {
            if (push_counter < 1) {
                EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_OUT);
            }

            bin_code[current_element++] = INT_OUT;

            PRINT_TEXT(text_stream, INT_OUT);
            push_counter--;
        }
//-------------------------------------------------------------------------------------
        else if (strcmp(comand, STR_MASS_COMANDS[INT_HLT]) == 0) {
            bin_code[current_element++] = INT_HLT;

            amount_cmd++;

            PRINT_TEXT(text_stream, INT_HLT);
            break;
        }
//-------------------------------------------------------------------------------------
        else {
            EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, UNKNOWN_CMD);
        }
        amount_cmd++;
    }
    // В надежде на то, что current_element поместится в <int>
    bin_code[0] = (int) current_element;
    bin_code[1] = max_push;

    fclose_file(asm_stream);

    FILE* bin_file = fopen_file(name_byte_file, "wb");
    if (fwrite(bin_code, sizeof(int), current_element, bin_file) != current_element) {
        printf("ERROR: write bin_code to bin_file was failed\n");
        free(bin_code);
        return -1;
    }
    fclose_file(bin_file);

    free(bin_code);
    free(asm_code);
    printf("Compilation was completed\n");
    return 0;
}


int find_mean_variable(const char* name_var, variable* name_table) {
    assert(name_var);
    assert(name_table);

    for (int i = 0; i < AMOUNT_VARIABLES; i++) {
        if (strcmp(name_table[i].name_variable, name_var + 1) == 0) {
            return name_table[i].mean_variable;
        }
    }

//TODO Добавить передачу переменой по адресу
    return 0;
}



int completion_name_table(variable* name_table) {
    assert(name_table);

// Первый коэффициент
    name_table[0].mean_variable = 1;
    name_table[0].name_variable = "A";

// Второй коэффициент
    name_table[1].mean_variable = 4;
    name_table[1].name_variable = "B";

// Третий коэффициент
    name_table[2].mean_variable = 4;
    name_table[2].name_variable = "C";

    return 0;
}



size_t skip_comment(const char* asm_code, const char last_char) {
    assert(asm_code);

    if (*asm_code != COMMENT_CHAR) {
        return 0;
    }

    return find_char(asm_code, last_char) + 1;
}


int check_realloc(int** bin_code, size_t* max_elements, size_t current_element) {
    assert(bin_code);
    assert(max_elements);

    if (current_element > *max_elements - REALLOC_CONST) {
        int* buffer_address = realloc_buffer(*bin_code, *max_elements);

        if (buffer_address == NULL) {
            printf("ERROR in check_realloc: Null address\n");
            return -1;
        }
        // printf("BUFFER = [%p]\n", buffer_address);
        // printf("BIN = [%p]\n", *bin_code);

        *bin_code = buffer_address;

        *max_elements *= MOD_REALLOC;
    }
    return 0;
}

