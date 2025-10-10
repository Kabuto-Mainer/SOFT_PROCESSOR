#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "../COMMON/config.h"
#include "../COMMON/color.h"

#define PRINT_TO_TEXT_FILE ON

#if PRINT_TO_TEXT_FILE == ON
#define PRINT_TEXT(text_stream, INT_CMD) fprintf(text_stream, "%d\n", INT_CMD);

#else
#define PRINT_TEXT(text_stream, INT_CMD) (void(0))
#endif

struct asm_struct {
    const char* name_file;
    FILE* text_stream;
    int* bin_code;
    char* asm_code;
    int* table_point;

    int amount_line;
    int cur_char;
    int cur_element;
    int hash_cmd;
};

#define EXIT_FUNCTION(data, return_error) \
 printf(_R_ "%s:%d: ERROR: %s\n" _N_, data->name_file, data->amount_line + 1, DESCRIPTION_ERRORS[return_error]); \
 return return_error;


enum asm_error_t{
    NOT_ERRORS = 0,
    FEW_PUSH = 1,
    NO_ARG = 2,
    UNKNOWN_REG = 3,
    NO_ADR = 4,
    NULL_ADR = 5,
    INVALID_POINT = 6,
    FILE_ERROR = 7,
    NO_REG = 8,
    LIMIT = 9,
    UNKNOWN_CMD = 10,
    ERROR = 11
};

char DESCRIPTION_ERRORS[][40] = {
    "no errors in code\n",
    "too few argument to push\n",
    "no register to use\n",
    "unknown register\n",
    "no address for jump\n",
    "null address in file\n",
    "not correct use point\n",
    "error with file\n",
    "no register to use\n",
    "too big or small argument\n",
    "unknown comand\n",
    "ERROR\n"
};


asm_error_t my_assembler (const char* name_asm_file,
                          const char* name_byte_file,
                          const char* name_text_file,
                          int* table_point);

asm_error_t check_J_cmd(asm_struct* asm_data, int cmd);

int cmd_to_hash(const char* comand);

asm_error_t check_comand(asm_struct* asm_data);

int check_realloc(int** bin_code, size_t* max_size, size_t current_size);


int char_reg_to_int(const char* name_reg);



#define CONTINUE \
 amount_cmd++; \
 amount_line++; \
 current_symbol += add_index + 1; \
 continue;


#define ADD_J_CMD(INT_COMAND) \
 if (strcmp(comand, CHAR_CMD[INT_COMAND]) == 0) { \
    int C_E = 0; \
    char buffer[10] = {}; \
    if (sscanf(asm_code + current_symbol + sscanf_amount, "%s", buffer) != 1) { \
        EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_LOC); \
    } \
    if (buffer[0] == ':') { \
        C_E = atoi(buffer + 1); \
        if (C_E >= AMOUNT_POINTS || C_E < 0) { \
            EXIT_FUNCTION(name_asm_file, amount_line, bin_code, asm_code, FEW_LOC); \
        } \
        C_E = table_point[C_E]; \
    } \
    else { \
        C_E = atoi(buffer); \
    } \
 \
    bin_code[current_element++] = INT_COMAND; \
    bin_code[current_element++] = C_E; \
 \
    fprintf(text_stream, "%d %d\n", INT_COMAND, C_E); \
    CONTINUE; \
}

// #define ADD_J_CMD(comand, INT_COMAND, asm_code, current_symbol, sscanf_amount, current_element, bin_code, name_asm_file)
//  else if (strcmp(comand, CHAR_CMD[INT_JB]) == 0) {
//     int C_E = 0;
//     if (sscanf(asm_code + current_symbol + sscanf_amount, "%d", &C_E) != 1) {
//         EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, FEW_LOC);
//     }
//     if (C_E < 0 || (size_t) C_E >= current_element) {
//         EXIT_FUNCTION(name_asm_file, amount_cmd, bin_code, asm_code, INVALID_LOC);
//     }
//
//     bin_code[current_element++] = INT_JB;
//     bin_code[current_element++] = C_E;
//
//     fprintf(text_stream, "%d %d\n", INT_JMP, C_E);
// }





#endif // DISASSEMBLER_H
