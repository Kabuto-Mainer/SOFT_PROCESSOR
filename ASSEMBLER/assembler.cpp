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


int main(int argv, char* args[])
{
    label_t table_label[AMOUNT_POINTS] = {};

    for (int i = 0; i < AMOUNT_POINTS; i++) {
        table_label[i] = {"\0", -1};
    }

    display_t disp_set = {-1, -1};
    disp_set.len = LEN_DISPLAY;
    disp_set.high = HIGH_DISPLAY;
    disp_set.size = LEN_DISPLAY * HIGH_DISPLAY;

    const char* name_input_file = NAME_ASM_FILE;
    const char* name_output_file = NAME_BIN_FILE;

    printf("ARG: %d\n", argv);
    check_arg(&disp_set,
              &name_input_file,
              &name_output_file,
              argv,
              args);

    my_assembler(name_input_file, name_output_file, NAME_TEXT_FILE, table_label, 0, &disp_set);
    my_assembler(name_input_file, name_output_file, NAME_TEXT_FILE, table_label, 1, &disp_set);

    return 0;
}

int check_arg(display_t* disp_set,
              const char** name_if,
              const char** name_of,
              int argv,
              char* args[])
{
    assert(disp_set);
    assert(name_if);
    assert(name_of);

    if (argv > 1) {
        for (int i = 1; i < argv; i++) {
            int len_flag = 0;

            if ((len_flag = check_flag(args[i], FLAGS[F_INP_FILE])) != -1) {
                *name_if = &(args[i][len_flag]);
                printf("NAME_INPUT_FILE: %s\n", *name_if);
            }

            else if ((len_flag = check_flag(args[i], FLAGS[F_OUT_FILE])) != -1) {
                *name_of = &(args[i][len_flag]);
                printf("NAME_INPUT_FILE: %s\n", *name_if);
            }

            else if ((len_flag = check_flag(args[i], FLAGS[F_SIZE_DISP])) != -1) {
                int buffer_len = -1;
                int buffer_high = -1;

                if (sscanf(args[i] + len_flag, "%d:%d", &buffer_len, &buffer_high) != 2 ||
                    buffer_len <= 0 || buffer_high <= 0) {
                    printf("ERROR: bad flag-size. Using standard settings\n");
                }

                else {
                    disp_set->len = buffer_len;
                    disp_set->high = buffer_high;
                    disp_set->size = buffer_len * buffer_high;
                    printf("Use size: %d:%d\n", buffer_len, buffer_high);
                }
            }
        }
    }
    return 0;
}

int check_flag(const char* input_arg,
               const char* flag) {
    assert(input_arg);
    assert(flag);

    size_t len_flag = strlen(flag);

    for (size_t i = 0; i < len_flag - 1; i++) {
        if (input_arg[i] != flag[i]) {
            return -1;
        }
    }

    return (int) len_flag;
}


int check_realloc(int** bin_code, size_t* max_elements, size_t current_element)
{
    assert(bin_code);
    assert(max_elements);

    if (current_element > *max_elements - REALLOC_CONST) {
        int* buffer_address = realloc_buffer(*bin_code, *max_elements);

        if (buffer_address == NULL) {
            printf(_R_ "ERROR in check_realloc: Null address\n" _N_);
            return -1;
        }
        *bin_code = buffer_address;

        *max_elements *= MOD_REALLOC;
    }
    return 0;
}


int char_reg_to_int(const char* name_reg)
{
    assert(name_reg);

    for (int i = 0; i < AMOUNT_REGISTERS; i++) {
        if (strcmp(name_reg, CHAR_REG[i]) == 0) {
            return i;
        }
    }

    return -1;
}

int asm_check_label(asm_struct* asm_data,
                  char* comand,
                  int number_start,
                  int add_index)
{
    assert(asm_data);

    if (number_start != 0) {
        asm_data->amount_line++;
        asm_data->cur_char += add_index + 1;

        return 0;
    }

    for (int i = 0; i < AMOUNT_POINTS; i++) {
        if (asm_data->table_label[i].name[0] != ':') {
            strcpy(asm_data->table_label[i].name, comand);
            asm_data->table_label[i].address = asm_data->cur_element;
            break;
        }
    }
    fprintf(asm_data->text_stream, "::: ");

    return 1;
}

int asm_check_var(asm_struct* asm_data)
{
    char name[100] = "";
    int value = 0;
    sscanf(asm_data->asm_code + asm_data->cur_char, "%s %d", name, &value);

    for (int i = 0; i < AMOUNT_VARS; i++) {
        if (asm_data->table_var[i].name[0] == 0) {
            strcpy(asm_data->table_var[i].name, name);
            asm_data->table_var[i].value = value;
            break;
        }

        if (i == AMOUNT_VARS - 1) {
            printf("ERROR: too many variables\n");
            return -1;
        }
    }
    fprintf(asm_data->text_stream, "### ");
    return 0;
}

int asm_skip_comment(asm_struct* asm_data,
                     char* comand,
                     int add_index)
{
    assert(asm_data);

    if (sscanf(asm_data->asm_code + asm_data->cur_char, "%39s", comand) == 0) {
        asm_data->amount_line++;
        asm_data->cur_char += add_index + 1;
        return 0;
    }

    if (comand[0] == ';' || comand[0] == '\0') {
        asm_data->amount_line++;
        asm_data->cur_char += add_index + 1;
        return 0;
    }
    return 1;
}



asm_error_t my_assembler (const char* name_asm_file,
                          const char* name_byte_file,
                          const char* name_text_file,
                          label_t* table_label,
                          int number_start,
                          display_t* disp_set)
{
    assert(name_asm_file);
    assert(name_byte_file);
    assert(name_text_file);
    assert(table_label);

    asm_struct asm_data = {};
    variable_t table_var[AMOUNT_VARS] = {};

    int amount_chars = asm_create(&asm_data,
               name_asm_file,
               name_text_file,
               table_label,
               table_var);

    size_t max_elements = START_AMOUNT_CMD;
    fprintf(asm_data.text_stream, "[ADR] | COMAND  ARGUMENT | ASM CODE\n");

    while (asm_data.cur_char < amount_chars)
    {
        if (check_realloc(&(asm_data.bin_code), &max_elements, (size_t) asm_data.cur_element) == -1)
        {
            free(asm_data.bin_code);
            free(asm_data.asm_code);
            fclose_file(asm_data.text_stream);
            return NULL_ADR;
        }

        int add_index = (int) find_char(asm_data.asm_code + asm_data.cur_char, '\n');
        *(asm_data.asm_code + asm_data.cur_char + add_index) = '\0';

        char comand[40] = "";

        if (asm_skip_comment(&asm_data, comand, add_index) == 0)
        {
            continue;
        }

        if (comand[0] == ':')
        {
            if (asm_check_label(&asm_data, comand, number_start, add_index) == 0)
            {
                continue;
            }
        }

        else if (comand[0] == '#')
        {
            if (asm_check_var(&asm_data) == -1) {
                return ERROR;
            }
        }

        else
        {
            fprintf(asm_data.text_stream, "[%3d] |", asm_data.cur_element - AMOUNT_SUP_NUM + 1);

            asm_data.hash_cmd = cmd_to_hash(comand);

            if (asm_check_cmd(&asm_data) != A_NOT_ERRORS) {
                free(asm_data.bin_code);
                free(asm_data.asm_code);
                return ERROR;
            }
        }



//         if (comand[0] == ':')
//         {
//             if (number_start != 0) {
//                 asm_data.amount_line++;
//                 asm_data.cur_char += add_index + 1;
//
//                 continue;
//             }
//
//             for (int i = 0; i < AMOUNT_POINTS; i++) {
//                 if (((table_label[i]).name)[0] != ':') {
//                     strcpy((table_label[i]).name, comand);
//                     (table_label[i]).address = asm_data.cur_element;
//                     break;
//                 }
//             }
//             fprintf(asm_data.text_stream, "::: ");
//         }

//         else if (comand[0] == '#') {
//             char name[100] = "";
//             int value = 0;
//             sscanf(asm_data.asm_code + asm_data.cur_char, "%s %d", name, &value);
//
//             for (int i = 0; i < AMOUNT_VARS; i++) {
//                 if (((table_var[i]).name)[0] == 0) {
//                     strcpy((table_var[i]).name, name);
//                     (table_var[i]).value = value;
//                     break;
//                 }
//
//                 if (i == AMOUNT_VARS - 1) {
//                     printf("ERROR: too many variables\n");
//                     return ERROR;
//                 }
//             }
//             fprintf(asm_data.text_stream, "### ");
//         }

//         else {
//             fprintf(asm_data.text_stream, "[%3d] |", asm_data.cur_element - AMOUNT_SUP_NUM + 1);
//
//             asm_data.hash_cmd = cmd_to_hash(comand);
//
//             if (asm_check_cmd(&asm_data) != A_NOT_ERRORS) {
//                 free(asm_data.bin_code);
//                 free(asm_data.asm_code);
//                 return ERROR;
//             }
//         }

        fprintf(asm_data.text_stream, " %s\n", asm_data.asm_code + asm_data.cur_char);

        asm_data.cur_char += add_index + 1;
        asm_data.amount_line++;

    }
    asm_data.bin_code[0] = OWN_SIGNATURE;
    asm_data.bin_code[1] = CURRENT_VERSION;
    asm_data.bin_code[2] = asm_data.cur_element;
    asm_data.bin_code[3] = disp_set->len;
    asm_data.bin_code[4] = disp_set->high;


    FILE* bin_file = fopen_file(name_byte_file, "wb");
    if (fwrite(asm_data.bin_code, sizeof(int), (size_t) asm_data.cur_element, bin_file) != (size_t) asm_data.cur_element)
    {
        printf(_R_ "ERROR: write bin_code to bin_file was failed\n" _N_);
        free(asm_data.bin_code);
        free(asm_data.asm_code);
        return FILE_ERROR;
    }
    fclose_file(bin_file);
    fclose_file(asm_data.text_stream);

    free(asm_data.bin_code);
    free(asm_data.asm_code);
    printf(_G_ "Compilation was completed\n" _N_);

    return A_NOT_ERRORS;
}

asm_error_t asm_check_cmd(asm_struct* asm_data)
{
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
                    for (size_t i_2 = 0; i_2 < AMOUNT_VARS; i_2++) {
                        if (strcmp((asm_data->table_var[i_2]).name, arg_c) == 0) {
                            arg = (asm_data->table_var)[i_2].value;
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

            else if ((CMD_INF[i]).type_arg == REG) {
                char char_reg[4] = "";
                char trash[100] = "";

                if (sscanf(asm_data->asm_code + asm_data->cur_char, "%s %s", trash, char_reg) == 1) {
                    EXIT_FUNCTION(asm_data, NO_REG);
                }

                int reg = 0;
                if ((reg = char_reg_to_int(char_reg)) == -1) {
                    EXIT_FUNCTION(asm_data, UNKNOWN_REG); \
                }

                asm_data->bin_code[asm_data->cur_element++] = (CMD_INF[i]).number;
                asm_data->bin_code[asm_data->cur_element++] = reg;
                \
                fprintf(asm_data->text_stream, "%08X %08X |", (unsigned int) (CMD_INF[i]).number, (unsigned int) reg);

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

    // printf("CHAR: %s\n", char_adr);

    if (char_adr[0] == ':') {
        for (int i = 0; i < AMOUNT_POINTS; i++) {
            // printf("TABLE_NAME: %s, INPUT_NAME: %s\n", asm_data->table_label[i].name, char_adr);

            if (strcmp((asm_data->table_label[i]).name, char_adr) == 0) {
                new_C_E = (asm_data->table_label[i]).address - AMOUNT_SUP_NUM + 1;
                // printf("NAME: %s, ADR: %d\n", asm_data->table_label[i].name, asm_data->table_label[i].address);
                break;
            }
            // printf("NAME: %s, ADR: %d\n", asm_data->table_label[i].name, asm_data->table_label[i].address);
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

    fprintf(asm_data->text_stream, "%08X %08d |", (unsigned int) cmd, (int) new_C_E);
    return A_NOT_ERRORS;
}

int sign_to_j_cmd(const char* string)
{
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

int asm_create(asm_struct* asm_data,
               const char* name_asm_file,
               const char* name_text_file,
               label_t* table_label,
               variable_t* table_var)
{

    asm_data->name_file = name_asm_file;
    asm_data->table_label = table_label;
    asm_data->amount_line = 0;
    asm_data->cur_char = 0;
    asm_data->cur_element = AMOUNT_SUP_NUM;

    size_t max_elements = START_AMOUNT_CMD;

    FILE* asm_stream = fopen_file(name_asm_file, "rb");
    FILE* text_stream = fopen_file(name_text_file, "w");

    char* asm_code = create_char_buffer(file_size_check(name_asm_file) + 1);
    if (asm_code == NULL) {
        return -1;
    }

    int amount_chars = (int) fread(asm_code, sizeof(char), file_size_check(name_asm_file), asm_stream);
    asm_code[amount_chars] = '\0';
    fclose_file(asm_stream);

    for (int i = 0; i < AMOUNT_VARS; i++) {
        table_var[i] = {0, 0};
    }
    asm_data->table_var = &(table_var[0]);

    int* bin_code = create_int_buffer(max_elements);

    asm_data->bin_code = bin_code;
    asm_data->asm_code = asm_code;
    asm_data->text_stream = text_stream;

    return amount_chars;
}
