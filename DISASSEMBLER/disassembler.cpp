#include <stdio.h>
#include <assert.h>

#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"

#include "disassembler.h"



int main()
{
    // my_textbin_to_asm(NAME_TEXT_FILE, NAME_DISASMB_FILE);
    my_bincode_to_asm(NAME_BIN_FILE, NAME_DISASMB_FILE);
    return 0;
}

int my_bincode_to_asm(const char* name_bin_file, const char* name_text_file)
{
    assert(name_bin_file);
    assert(name_text_file);

    FILE* bin_stream = fopen_file(name_bin_file, "rb");
    FILE* text_stream = fopen_file(name_text_file, "w");

    if (check_inf(bin_stream) != 0) {
        return -1;
    }

    size_t amount_elements = 0;
    if (fread(&amount_elements, sizeof(int), 1, bin_stream) != 1) {
        fclose_file(bin_stream);
        printf(_R_ "ERROR: read amount_elements not correct\n" _N_);
        return -1;
    }

    fseek(bin_stream, AMOUNT_SUP_NUM * sizeof(int), SEEK_SET);

    int* bin_code = create_int_buffer(amount_elements);
    if (bin_code == NULL) {
        return -1;
    }

    fread(bin_code, sizeof(int), amount_elements, bin_stream);

    fclose_file(bin_stream);


    for (unsigned int cur_el = 0; cur_el < amount_elements - 2; cur_el++) {
        if (bin_code[cur_el] == INT_PUSH ||
            bin_code[cur_el] == INT_JMP ||
            bin_code[cur_el] == INT_JB ||
            bin_code[cur_el] == INT_JBE ||
            bin_code[cur_el] == INT_JA ||
            bin_code[cur_el] == INT_JAE ||
            bin_code[cur_el] == INT_JE ||
            bin_code[cur_el] == INT_JNE ||
            bin_code[cur_el] == INT_CALL ||
            bin_code[cur_el] == INT_PAINT) {
            fprintf(text_stream, "%s %d\n", CHAR_CMD[bin_code[cur_el]], bin_code[cur_el + 1]);
            cur_el++;
        }

        else if (bin_code[cur_el] == INT_POPR ||
                 bin_code[cur_el] == INT_PUSHR ||
                 bin_code[cur_el] == INT_PUSHM ||
                 bin_code[cur_el] == INT_POPM) {
            fprintf(text_stream, "%s %s\n", CHAR_CMD[bin_code[cur_el]], CHAR_REG[bin_code[cur_el + 1]]);
            cur_el++;
        }

        else {
            fprintf(text_stream, "%s\n", CHAR_CMD[bin_code[cur_el]]);
        }
    }
    fclose_file(text_stream);
    free(bin_code);
    return 0;
}

