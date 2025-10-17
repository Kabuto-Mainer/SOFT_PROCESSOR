#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"

#include "../PROCESSOR/stack.h"
#include "../PROCESSOR/stack_define.h"


int main(void) {
    label_t table_label[AMOUNT_POINTS] = {};

    for (int i = 0; i < AMOUNT_POINTS; i++) {
        table_label[i] = {0, -1};
    }

    my_assembler(NAME_ASM_FILE, NAME_BIN_FILE, NAME_TEXT_FILE, table_label);
    my_assembler(NAME_ASM_FILE, NAME_BIN_FILE, NAME_TEXT_FILE, table_label);

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
                          label_t* table_label)
{
    assert(name_asm_file);
    assert(name_byte_file);
    assert(name_text_file);
    assert(table_label);

    asm_struct asm_data = {};
    asm_data.name_file = name_asm_file;
    asm_data.table_label = table_label;
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

    variable_t table_var[AMOUNT_VARS] = {};
    for (int i = 0; i < AMOUNT_VARS; i++) {
        table_var[i] = {0, 0};
    }
    asm_data.table_var = &(table_var[0]);

    int* bin_code = create_int_buffer(max_elements);

    asm_data.bin_code = bin_code;
    asm_data.asm_code = asm_code;
    asm_data.text_stream = text_stream;

    fprintf(text_stream, "[ADR] | COMAND  ARGUMENT | ASM CODE\n");

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

        // printf("ALL: -%s-\n", asm_code + asm_data.cur_char);

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

        // fprintf(text_stream, "[%3d] ", asm_data.cur_element - AMOUNT_SUP_NUM + 1);

        if (comand[0] == ':') {
            for (int i = 0; i < AMOUNT_POINTS; i++) {
                if (((table_label[i]).name)[0] == 0) {
                    strcpy((table_label[i]).name, comand);
                    (table_label[i]).address = asm_data.cur_element;
                }
            }

            // fputc('*', text_stream);
            fprintf(text_stream, "::: ");
        }

        else if (comand[0] == '#') {
            char name[100] = "";
            int value = 0;
            sscanf(asm_code + asm_data.cur_char, "%s %d", name, &value);

            for (int i = 0; i < AMOUNT_VARS; i++) {
                if (((table_var[i]).name)[0] == 0) {
                    strcpy((table_var[i]).name, name);
                    (table_var[i]).value = value;
                    break;
                }

                if (i == AMOUNT_VARS - 1) {
                    printf("ERROR: too many variables\n");
                    return ERROR;
                }
            }
            fprintf(text_stream, "### ");
        }

        else {
            fprintf(text_stream, "[%3d] |", asm_data.cur_element - AMOUNT_SUP_NUM + 1);

            asm_data.hash_cmd = cmd_to_hash(comand);

            if (check_comand(&asm_data) != A_NOT_ERRORS) {
                free(bin_code);
                free(asm_code);
                return ERROR;
            }
        }

        fprintf(text_stream, " %s\n", asm_data.asm_code + asm_data.cur_char);
        // for (int i = add_index; i < 60; i++) {
        //     fputc(' ', text_stream);
        // }

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

    return A_NOT_ERRORS;
}

//
// asm_error_t check_comand(asm_struct* asm_data)
// {
//     assert(asm_data);
//
//     switch (asm_data->hash_cmd)
//     {
//         case (HASH_PUSH): {
//             char arg_c[100] = "";
//             char trash[100] = "";
//             int arg = 0;
//
//             if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %s", trash, arg_c) == 1) {
//                 EXIT_FUNCTION(asm_data, NO_ARG);
//             }
//
//             if (arg_c[0] == '#') {
//                 for (int i = 0; i < AMOUNT_VARS; i++) {
//                     if (strcmp((asm_data->table_var[i]).name, arg_c) == 0) {
//                         arg = (asm_data->table_var)->value;
//                         break;
//                     }
//
//                     if (i == AMOUNT_VARS - 1) {
//                         EXIT_FUNCTION(asm_data, NO_VAR);
//                     }
//                 }
//             }
//
//             else {
//                 arg = atoi(arg_c);
//
//                 if (arg == 0 && arg_c[0] != '0') {
//                     EXIT_FUNCTION(asm_data, NO_INT);
//                 }
//             }
//
//             if (arg < MIN_MEAN || arg > MAX_MEAN) {
//                 EXIT_FUNCTION(asm_data, LIMIT);
//             }
//
//             asm_data->bin_code[asm_data->cur_element++] = INT_PUSH;
//             asm_data->bin_code[asm_data->cur_element++] = arg;
//
//             fprintf(asm_data->text_stream, "%08X %08X |", (unsigned int) INT_PUSH, (unsigned int) arg);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_POPR): {
//             CMD_WITH_REG(INT_POPR);
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_PUSHR): {
//             CMD_WITH_REG(INT_PUSHR);
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_ADD): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_ADD;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_ADD);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_SUB): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_SUB;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_SUB);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_MUL): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_MUL;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_MUL);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_DIV): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_DIV;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_DIV);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_SQRT): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_SQRT;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_SQRT);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_OUT): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_OUT;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_OUT);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_IN): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_IN;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_IN);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JMP): {
//             if (check_J_cmd(asm_data, INT_JMP) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JB): {
//             if (check_J_cmd(asm_data, INT_JB) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JBE): {
//             if (check_J_cmd(asm_data, INT_JBE) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JA): {
//             if (check_J_cmd(asm_data, INT_JA) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JAE): {
//             if (check_J_cmd(asm_data, INT_JAE) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JE): {
//             if (check_J_cmd(asm_data, INT_JE) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_JNE): {
//             if (check_J_cmd(asm_data, INT_JNE) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_HACK): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_HACK;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_HACK);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_HLT): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_HLT;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_HLT);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_CALL): {
//             if (check_J_cmd(asm_data, INT_CALL) != A_NOT_ERRORS) {
//                 return ERROR;
//             }
//
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_RET): {
//             asm_data->bin_code[asm_data->cur_element++] = INT_RET;
//
//             fprintf(asm_data->text_stream, "%08X          |", (unsigned int) INT_RET);
//             break;
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_PUSHM): {
//             CMD_WITH_REG(INT_PUSHM);
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_POPM): {
//             CMD_WITH_REG(INT_POPM);
//         }
// //-----------------------------------------------------------------------------------------------------
//         case (HASH_PAINT): {
//             CMD_WITH_REG(INT_PAINT);
//         }
// //-----------------------------------------------------------------------------------------------------
//         default: {
//             EXIT_FUNCTION(asm_data, UNKNOWN_CMD);
//         }
//     }
//
//     return A_NOT_ERRORS;
// }


asm_error_t check_comand(asm_struct* asm_data) {
    assert(asm_data);

    for (size_t i = 0; i < sizeof(CMD_INF) / sizeof(CMD_INF[0]); i++) {
        if (asm_data->hash_cmd == (CMD_INF[i]).hash_cmd) {
            if ((CMD_INF[i]).amount_arg == 0) {
                asm_data->bin_code[asm_data->cur_element++] = (CMD_INF[i]).number;

                fprintf(asm_data->text_stream, "%08X          |",
                       (unsigned int) (CMD_INF[i]).number);

                return A_NOT_ERRORS;
            }

            else if ((CMD_INF[i]).type_arg == NUM) {
                char arg_c[100] = "";
                char trash[100] = "";
                int arg = 0;

                if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %s", trash, arg_c) == 1) {
                    EXIT_FUNCTION(asm_data, NO_ARG);
                }

                if (arg_c[0] == '#') {
                    for (size_t i_2 = 0; i < AMOUNT_VARS; i++) {
                        if (strcmp((asm_data->table_var[i_2]).name, arg_c) == 0) {
                            arg = (asm_data->table_var)->value;
                            break;
                        }

                        if (i_2 == AMOUNT_VARS - 1) {
                            EXIT_FUNCTION(asm_data, NO_VAR);
                        }
                    }
                }

                else {
                    arg = atoi(arg_c);

                    if (arg == 0 && arg_c[0] != '0') {
                        EXIT_FUNCTION(asm_data, NO_INT);
                    }
                }

                if (arg < MIN_MEAN || arg > MAX_MEAN) {
                    EXIT_FUNCTION(asm_data, LIMIT);
                }

                asm_data->bin_code[asm_data->cur_element++] = (CMD_INF[i]).number;
                asm_data->bin_code[asm_data->cur_element++] = arg;

                fprintf(asm_data->text_stream, "%08X %08X |",
                       (unsigned int) (CMD_INF[i]).number,
                       (unsigned int) arg);

                return A_NOT_ERRORS;
            }

            else if ((CMD_INF[i]).type_arg == ADR) {
                if (check_J_cmd(asm_data, (CMD_INF[i]).number) != A_NOT_ERRORS) {
                    return ERROR;
                }

                return A_NOT_ERRORS;
            }
        }
    }

    return ERROR;
}

asm_error_t check_J_cmd(asm_struct* asm_data, int cmd)
{
    assert(asm_data);
    assert(asm_data->table_label);

    char char_adr[20] = "";
    char trash[100] = "";
    int new_C_E = 0;


    if (sscanf(asm_data->asm_code + asm_data->cur_char + 1, "%s %s", trash, char_adr) == 1) {
        EXIT_FUNCTION(asm_data, NO_ADR);
    }

    char_adr[19] = '\0'; // На всякий

    if (char_adr[0] == ':') {
        for (int i = 0; i < AMOUNT_POINTS; i++) {
            if (strcmp((asm_data->table_label[i]).name, char_adr) == 0) {
                new_C_E = (asm_data->table_label[i]).address - AMOUNT_SUP_NUM + 1;
            }
        }
    }

    else if (char_adr[0] == '$') {
        int add_num = atoi(char_adr + 1);
        if (add_num < 0) {
            EXIT_FUNCTION(asm_data, INVALID_POINT);
        }

        new_C_E = asm_data->cur_element + add_num - 2;
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

    fprintf(asm_data->text_stream, "%08X %08X |", (unsigned int) cmd, (unsigned int) new_C_E);
    return A_NOT_ERRORS;
}

int sign_to_j_cmd(const char* string) {
    assert(string);

    if (string[0] == '<') {
        if (string[1] == '=') {
            return INT_JBE;
        }

        else {
            return INT_JB;
        }
    }

    if (string[0] == '>') {
        if (string[1] == '=') {
            return INT_JAE;
        }

        else {
            return INT_JA;
        }
    }

    if (string[1] == '=') {
        if (string[0] == '=') {
            return INT_JE;
        }

        else if (string[0] == '!') {
            return INT_JNE;
        }

        else {
            return -1;
        }
    }

    return -1;
}


