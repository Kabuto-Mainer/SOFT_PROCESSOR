#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>

#include "assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"

int main(void) {

    my_assembler(NAME_ASM_FILE, NAME_BIN_FILE, NAME_TEXT_FILE);

    return 0;
}


asm_error_t my_assembler(const char* name_asm_file,
                const char* name_byte_file,
                const char* name_text_file) {
    assert(name_asm_file);
    assert(name_byte_file);
    assert(name_text_file);

    FILE* asm_stream = fopen_file(name_asm_file, "rb");

    FILE* text_stream = fopen_file(name_text_file, "w");

    char* asm_code = create_char_buffer(file_size_check(name_asm_file) + 1);
    if (asm_code == NULL) {
        return NULL_ADR;
    }

    size_t amount_symbols = fread(asm_code, sizeof(char), file_size_check(name_asm_file), asm_stream);
    asm_code[amount_symbols] = '\0';
    size_t current_symbol = 0;

    int push_counter = 0; // Текущее количество элементов в стеке
    int max_push = 0; // Максимальное количество элементов в стеке
    int amount_cmd = 0;
    int amount_line = 0;

    size_t max_elements = START_AMOUNT_CMD; // Only >= current_element
    size_t current_element = AMOUNT_SUP_NUM; // Only ++

    int all_reg[AMOUNT_REGISTERS] = {0};

    // bin_code = <amount_cmd> <max_push> <bin_code[2]> ...
    int* bin_code = create_int_buffer(START_AMOUNT_CMD);


    while (current_symbol <= amount_symbols) {
        char comand[20] = "";

        size_t add_index = find_char(asm_code + current_symbol, ';');

        if (*(asm_code + current_symbol + add_index) == ';') {
            *(asm_code + current_symbol + add_index) = '\0';

            add_index += find_char(asm_code + current_symbol + add_index + 1, '\n') + 1;
        }
        else {
            *(asm_code + current_symbol + add_index) = '\0';
        }

        int sscanf_amount = 0;
        if (*(asm_code + current_symbol) == '\0' ||
            sscanf(asm_code + current_symbol, "%s %n", comand, &sscanf_amount) == 0) {
            current_symbol += add_index + 1;
            amount_line++;
            continue;
        }
        comand[19] = '\0'; // На всякий случай

        // printf("COMAND: %s\n", comand);
// TODO struct

// Проверки размеров
//-----------------------------------------------------------------
        if (check_realloc(&bin_code, &max_elements, current_element) == -1) {
            free(bin_code);
            free(asm_code);
            return NULL_ADR;
        }

        if (max_push < push_counter) {
            max_push = push_counter;
        }
//--------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_PUSH]) == 0) {
            int argument = 0;
            if (sscanf(asm_code + current_symbol + sscanf_amount - 1, "%d", &argument) != 1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, NO_ARG);
            }

            // printf("ARGUMENT TO PUSH: %d\n", argument);
            if (argument < MIN_MEAN || argument > MAX_MEAN) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, LIMIT);
            }

            bin_code[current_element++] = INT_PUSH;
            bin_code[current_element++] = argument;

            fprintf(text_stream, "%d %d\n", INT_PUSH, argument);
            push_counter++;

            CONTINUE;
        }
//---------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_IN]) == 0) {
            bin_code[current_element++] = INT_IN;

            PRINT_TEXT(text_stream, INT_IN);
            push_counter++;

            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_POPR]) == 0) {
            char REG[10] = "";
            if (sscanf(asm_code + current_symbol + sscanf_amount, "%9s", REG) != 1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_REG);
            }
            REG[9] = '\0';

            int buffer = 0;
            buffer = char_reg_to_int(REG);

            if (buffer == -1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, INVALID_REG);
            }

            all_reg[buffer] = 1;

            bin_code[current_element++] = INT_POPR;
            bin_code[current_element++] = buffer;

            fprintf(text_stream, "%d %d\n", INT_POPR, buffer);
            push_counter--;

            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_PUSHR]) == 0) {
            char REG[10] = "";
            if (sscanf(asm_code + current_symbol + sscanf_amount, "%9s", REG) != 1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_REG);
            }

            int buffer = 0;
            buffer = char_reg_to_int(REG);

            if (buffer == -1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, INVALID_REG);
            }

            if (all_reg[buffer] == 0) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, NO_VARIABLE);
            }

            bin_code[current_element++] = INT_PUSHR;
            bin_code[current_element++] = buffer;

            fprintf(text_stream, "%d %d\n", INT_PUSHR, buffer);
            push_counter++;

            CONTINUE;
        }
//---------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_ADD]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_ADD);
            }

            bin_code[current_element++] = INT_ADD;
            push_counter--;

            PRINT_TEXT(text_stream, INT_ADD);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_SUB]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_SUB);
            }

            bin_code[current_element++] = INT_SUB;
            push_counter--;

            PRINT_TEXT(text_stream, INT_SUB);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_DIV]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_DIV);
            }

            bin_code[current_element++] = INT_DIV;
            push_counter--;

            PRINT_TEXT(text_stream, INT_DIV);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_MUL]) == 0) {
            if (push_counter < 2) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_MUL);
            }

            bin_code[current_element++] = INT_MUL;
            push_counter--;

            PRINT_TEXT(text_stream, INT_MUL);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_SQRT]) == 0) {
            if (push_counter < 1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_SQRT);
            }
            bin_code[current_element++] = INT_SQRT;

            PRINT_TEXT(text_stream, INT_SQRT);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        ADD_J_CMD(INT_JMP);
        ADD_J_CMD(INT_JB);
        ADD_J_CMD(INT_JBE);
        ADD_J_CMD(INT_JA);
        ADD_J_CMD(INT_JAE);
        ADD_J_CMD(INT_JE);
        ADD_J_CMD(INT_JNE);
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_OUT]) == 0) {
            if (push_counter < 1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_OUT);
            }

            bin_code[current_element++] = INT_OUT;

            push_counter--;
            PRINT_TEXT(text_stream, INT_OUT);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_HACK]) == 0) {
            bin_code[current_element++] = INT_HACK;

            PRINT_TEXT(text_stream, INT_HACK);
            CONTINUE;
        }
//----------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_CODE]) == 0) {
            int code = 0;
            int point = 0;
            if (sscanf(asm_code + current_symbol + sscanf_amount - 1, "%d", &code) != 1) {
                EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, NO_ARG);
            }

            bin_code[current_element++] = INT_CODE;

            for (int i = 0; i < AMOUNT_CODS; i++) {
                if (code == INT_CODS[i]) {
                    bin_code[current_element++] = i;
                    fprintf(text_stream, "%d %d\n", INT_CODE, i);

                    point = 1;
                    CONTINUE;
                }
            }
            if (point == 1) {
                continue;
            }
            EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, INVALID_CODE);
        }
//-------------------------------------------------------------------------------------
        if (strcmp(comand, CHAR_CMD[INT_HLT]) == 0) {
            bin_code[current_element++] = INT_HLT;

            PRINT_TEXT(text_stream, INT_HLT);
            CONTINUE;
        }
//-------------------------------------------------------------------------------------
        EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, UNKNOWN_CMD);
    }
    // В надежде на то, что current_element поместится в <int>
    bin_code[0] = OWN_SIGNATURE;
    bin_code[1] = CURRENT_VERSION;
    bin_code[2] = (int) current_element;
    bin_code[3] = max_push;

    fclose_file(asm_stream);

    FILE* bin_file = fopen_file(name_byte_file, "wb");
    if (fwrite(bin_code, sizeof(int), current_element, bin_file) != current_element) {
        printf(_R_ "ERROR: write bin_code to bin_file was failed\n" _N_);
        free(bin_code);
        return FILE_ERR;
    }
    fclose_file(bin_file);

    free(bin_code);
    free(asm_code);
    printf(_G_ "Compilation was completed\n" _N_);
    return NOT_ERRORS;
}


int check_realloc(int** bin_code, size_t* max_elements, size_t current_element) {
    assert(bin_code);
    assert(max_elements);

    if (current_element > *max_elements - REALLOC_CONST) {
        int* buffer_address = realloc_buffer(*bin_code, *max_elements);

        if (buffer_address == NULL) {
            printf(_R_ "ERROR in check_realloc: Null address\n" _N_);
            return -1;
        }
        // printf("BUFFER = [%p]\n", buffer_address);
        // printf("BIN = [%p]\n", *bin_code);

        *bin_code = buffer_address;

        *max_elements *= MOD_REALLOC;
    }
    return 0;
}


int char_reg_to_int(const char* name_reg) {
    assert(name_reg);

    for (int i = 0; i < AMOUNT_REGISTERS; i++) {
        if (strcmp(name_reg, CHAR_REG[i]) == 0) {
            return i;
        }
    }

    return -1;
}
