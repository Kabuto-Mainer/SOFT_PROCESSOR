#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"


int main(void) {
    int table_point[AMOUNT_POINTS] = {-1, -1, -1, -1, -1, -1, -1, -1};

//     table_point[0] = -2;
    // for (int i = 0; i < 19; i++) {
    //     printf("%s: %d\n", CHAR_CMD[i], cmd_to_hash(CHAR_CMD[i]));
    // }
//
    my_assembler(NAME_ASM_FILE, NAME_BIN_FILE, NAME_TEXT_FILE, table_point);

    // printf("%d\n", table_point[0]);

    my_assembler(NAME_ASM_FILE, NAME_BIN_FILE, NAME_TEXT_FILE, table_point);

    return 0;
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



asm_error_t my_assembler (const char* name_asm_file,
                          const char* name_byte_file,
                          const char* name_text_file,
                          int* table_point)
{
    assert(name_asm_file);
    assert(name_byte_file);
    assert(name_text_file);
    assert(table_point);

    asm_struct asm_data = {};
    asm_data.name_file = name_asm_file;
    asm_data.table_point = table_point;
    asm_data.amount_line = 0;
    asm_data.cur_char = 0;
    asm_data.cur_element = AMOUNT_SUP_NUM;

    size_t max_elements = START_AMOUNT_CMD;

    FILE* asm_stream = fopen_file(name_asm_file, "rb");
    FILE* text_stream = fopen_file(name_text_file, "w");

    char* asm_code = create_char_buffer(file_size_check(name_asm_file) + 1);
    if (asm_code == NULL) {
        return NULL_ADR;
    }

    int amount_chars = (int) fread(asm_code, sizeof(char), file_size_check(name_asm_file), asm_stream);
    asm_code[amount_chars] = '\0';
    fclose_file(asm_stream);

    int* bin_code = create_int_buffer(max_elements);

    asm_data.bin_code = bin_code;
    asm_data.asm_code = asm_code;
    asm_data.text_stream = text_stream;


    while (asm_data.cur_char < amount_chars) {
        if (check_realloc(&bin_code, &max_elements, (size_t) asm_data.cur_element) == -1) {
            free(bin_code);
            free(asm_code);
            fclose_file(text_stream);
            return NULL_ADR;
        }
        asm_data.bin_code = bin_code;

        int add_index = (int) find_char(asm_code + asm_data.cur_char, '\n');
        *(asm_code + asm_data.cur_char + add_index) = '\0';

        char comand[40] = "";

        if (sscanf(asm_code + asm_data.cur_char, "%39s", comand) == 0) {
            asm_data.amount_line++;
            asm_data.cur_char += add_index + 1;
            // printf("C_E: %d\n", asm_data.cur_element);
            continue;
        }

        if (comand[0] == ';' || comand[0] == '\0') {
            asm_data.amount_line++;
            asm_data.cur_char += add_index + 1;
            // printf("C_E: %d\n", asm_data.cur_element);

            continue;
        }

        // printf("CMD: -%s-\n", comand);

        fprintf(text_stream, "[%3d] %s ", asm_data.cur_element - AMOUNT_SUP_NUM + 1, asm_data.asm_code + asm_data.cur_char);
        for (int i = add_index; i < 60; i++) {
            fputc(' ', text_stream);
        }

        if (comand[0] == ':') {
            int point = 0;

            if (sscanf(asm_code + asm_data.cur_char + 1, "%d", &point) == 0) {
                printf(_R_ "%s:%d: ERROR: %s\n" _N_, name_asm_file, asm_data.amount_line + 1, DESCRIPTION_ERRORS[INVALID_POINT]);
                free(asm_data.bin_code);
                free(asm_data.asm_code);
                fclose(text_stream);
                return INVALID_POINT;
            }

            if (point < 0 || point >= AMOUNT_POINTS) {
                printf(_R_ "%s:%d: ERROR: %s\n" _N_, name_asm_file, asm_data.amount_line + 1, DESCRIPTION_ERRORS[INVALID_POINT]);
                free(asm_data.bin_code);
                free(asm_data.asm_code);
                fclose(text_stream);
                return INVALID_POINT;
            }
            // printf("POINT: %d\n", point);
            // printf("LINE: %d\n", asm_data.amount_line);
            table_point[point] = asm_data.cur_element;
            fputc('\n', text_stream);
        }

        else {
            asm_data.hash_cmd = cmd_to_hash(comand);

            if (check_comand(&asm_data) != NOT_ERRORS) {
                free(bin_code);
                free(asm_code);
                return ERROR;
            }
        }

        asm_data.cur_char += add_index + 1;
        asm_data.amount_line++;

    }
    bin_code[0] = OWN_SIGNATURE;
    bin_code[1] = CURRENT_VERSION;
    bin_code[2] = asm_data.cur_element;
    bin_code[3] = 10;

    FILE* bin_file = fopen_file(name_byte_file, "wb");
    if (fwrite(bin_code, sizeof(int), (size_t) asm_data.cur_element, bin_file) != (size_t) asm_data.cur_element) {
        printf(_R_ "ERROR: write bin_code to bin_file was failed\n" _N_);
        free(bin_code);
        free(asm_code);
        return FILE_ERROR;
    }
    fclose_file(bin_file);
    fclose_file(text_stream);

    free(bin_code);
    free(asm_code);
    printf(_G_ "Compilation was completed\n" _N_);

    return NOT_ERRORS;
}


asm_error_t check_comand(asm_struct* asm_data)
{
    assert(asm_data);

    switch (asm_data->hash_cmd)
    {
        case (HASH_PUSH): {
            int arg = 0;
            char trash[100] = "";

            if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %d", trash, &arg) == 1) {
                EXIT_FUNCTION(asm_data, NO_ARG);
            }

            if (arg < MIN_MEAN || arg > MAX_MEAN) {
                EXIT_FUNCTION(asm_data, LIMIT);
            }

            asm_data->bin_code[asm_data->cur_element++] = INT_PUSH;
            asm_data->bin_code[asm_data->cur_element++] = arg;

            fprintf(asm_data->text_stream, "%d %d\n", INT_PUSH, arg);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_POPR): {
            char char_reg[4] = "";
            char trash[100] = "";

            if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %s", trash, char_reg) == 1) {
                EXIT_FUNCTION(asm_data, NO_REG);
            }

            int reg = 0;
            if ((reg = char_reg_to_int(char_reg)) == -1) {
                EXIT_FUNCTION(asm_data, UNKNOWN_REG);
            }

            asm_data->bin_code[asm_data->cur_element++] = INT_POPR;
            asm_data->bin_code[asm_data->cur_element++] = reg;

            fprintf(asm_data->text_stream, "%d %d\n", INT_POPR, reg);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_PUSHR): {
            char char_reg[4] = "";
            char trash[100] = "";

            if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %s", trash, char_reg) == 1) {
                EXIT_FUNCTION(asm_data, NO_REG);
            }

            int reg = 0;
            if ((reg = char_reg_to_int(char_reg)) == -1) {
                EXIT_FUNCTION(asm_data, UNKNOWN_REG);
            }

            asm_data->bin_code[asm_data->cur_element++] = INT_PUSHR;
            asm_data->bin_code[asm_data->cur_element++] = reg;

            fprintf(asm_data->text_stream, "%d %d\n", INT_PUSHR, reg);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_ADD): {
            asm_data->bin_code[asm_data->cur_element++] = INT_ADD;

            fprintf(asm_data->text_stream, "%d\n", INT_ADD);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_SUB): {
            asm_data->bin_code[asm_data->cur_element++] = INT_SUB;

            fprintf(asm_data->text_stream, "%d\n", INT_SUB);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_MUL): {
            asm_data->bin_code[asm_data->cur_element++] = INT_MUL;

            fprintf(asm_data->text_stream, "%d\n", INT_MUL);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_DIV): {
            asm_data->bin_code[asm_data->cur_element++] = INT_DIV;

            fprintf(asm_data->text_stream, "%d\n", INT_DIV);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_SQRT): {
            asm_data->bin_code[asm_data->cur_element++] = INT_SQRT;

            fprintf(asm_data->text_stream, "%d\n", INT_SQRT);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_OUT): {
            asm_data->bin_code[asm_data->cur_element++] = INT_OUT;

            fprintf(asm_data->text_stream, "%d\n", INT_OUT);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_IN): {
            asm_data->bin_code[asm_data->cur_element++] = INT_IN;

            fprintf(asm_data->text_stream, "%d\n", INT_IN);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JMP): {
            if (check_J_cmd(asm_data, INT_JMP) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JB): {
            if (check_J_cmd(asm_data, INT_JB) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JBE): {
            if (check_J_cmd(asm_data, INT_JBE) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JA): {
            if (check_J_cmd(asm_data, INT_JA) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JAE): {
            if (check_J_cmd(asm_data, INT_JAE) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JE): {
            if (check_J_cmd(asm_data, INT_JE) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_JNE): {
            if (check_J_cmd(asm_data, INT_JNE) != NOT_ERRORS) {
                return ERROR;
            }

            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_HACK): {
            asm_data->bin_code[asm_data->cur_element++] = INT_HACK;

            fprintf(asm_data->text_stream, "%d\n", INT_HACK);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        case (HASH_HLT): {
            asm_data->bin_code[asm_data->cur_element++] = INT_HLT;

            fprintf(asm_data->text_stream, "%d\n", INT_HLT);
            break;
        }
//-----------------------------------------------------------------------------------------------------
        default: {
            EXIT_FUNCTION(asm_data, UNKNOWN_CMD);
        }
    }

    return NOT_ERRORS;
}


asm_error_t check_J_cmd(asm_struct* asm_data, int cmd)
{
    assert(asm_data);
    assert(asm_data->table_point);

    char char_adr[8] = "";
    char trash[100] = "";
    int new_C_E = 0;


    if (sscanf(asm_data->asm_code + asm_data->cur_char + 1, "%s %s", trash, char_adr) == 1) {
        EXIT_FUNCTION(asm_data, NO_ADR);
    }

    char_adr[7] = '\0'; // На всякий

    if (char_adr[0] == ':') {
        int point = atoi(char_adr + 1);
        if (point < 0 || point >= AMOUNT_POINTS) {
            EXIT_FUNCTION(asm_data, INVALID_POINT);
        }

        new_C_E = asm_data->table_point[point] - AMOUNT_SUP_NUM + 1;
        // printf("new_C_E: %d\n", new_C_E);
    }

    else if (isdigit(char_adr[0]) != 0) {
        new_C_E = atoi(char_adr);
    }

    else {
        EXIT_FUNCTION(asm_data, INVALID_POINT);
    }

    asm_data->bin_code[asm_data->cur_element++] = cmd;
    asm_data->bin_code[asm_data->cur_element++] = new_C_E;

    fprintf(asm_data->text_stream, "%d %d\n", cmd, new_C_E);
    return NOT_ERRORS;
}


int cmd_to_hash(const char* comand)
{
    assert(comand);

    int hash_cmd = 5183;

    for (int i = 0; i < 20; i++) {
        if (comand[i] == ' ' || comand[i] == '\0') {
            break;
        }

        hash_cmd += comand[i] * 33 + 13;
    }

    return hash_cmd;
}


