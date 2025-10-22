#include <stdio.h>
#include <assert.h>

#include "../ASSEMBLER/assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/config.h"
#include "file-name.h"
#include "test-func.h"


int main()
{
    for (int i = 0; i < 3; i++)
    {
        create_input_file(ASM_FILES[i],
                          INPUT_BIN_FILE,
                          LISTEN_FILE);

        if (check_bin_code(INPUT_BIN_FILE, T_BIN_FILES[i]) != 1)
        {
            printf("TESTING: error with %s\n", ASM_FILES[i]);
            return 0;
        }
    }
    printf("testing completed successfully\n");
    return 0;
}

int create_input_file(const char* name_asm_file,
                      const char* name_bin_file,
                      const char* name_listening)
{
    assert(name_asm_file);
    assert(name_listening);
    assert(name_bin_file);

    label_t table_label[AMOUNT_POINTS] = {};

    for (int i = 0; i < AMOUNT_POINTS; i++)
    {
        table_label[i] = {0, 0};
    }

    display_t disp_set = {-1, -1};
    disp_set.len = LEN_DISPLAY;
    disp_set.high = HIGH_DISPLAY;
    disp_set.size = LEN_DISPLAY * HIGH_DISPLAY;

    my_assembler(name_asm_file, name_bin_file, name_listening, table_label, 0, &disp_set);
    my_assembler(name_asm_file, name_bin_file, name_listening, table_label, 1, &disp_set);

    return 0;
}



int check_bin_code(const char* name_input_file,
                   const char* name_true_file)
{
    assert(name_input_file);
    assert(name_true_file);

    FILE* i_code_stream = fopen_file(name_input_file, "rb");
    FILE* t_code_stream = fopen_file(name_true_file, "rb");

    size_t size_bin_input = file_size_check(name_input_file);
    size_t size_bin_true = file_size_check(name_true_file);

    if (size_bin_input != size_bin_true)
    {
        return 0;
    }

    int* input_bin = create_int_buffer(size_bin_input);
    int* true_bin = create_int_buffer(size_bin_true);

    size_bin_true = fread(true_bin, sizeof(int), size_bin_true, t_code_stream);

    size_bin_input = fread(input_bin, sizeof(int), size_bin_input, i_code_stream);

    if (size_bin_input != size_bin_true)
    {
        free(true_bin);
        free(input_bin);

        return -1;
    }

    fclose_file(t_code_stream);
    fclose_file(i_code_stream);

    for (size_t i = 0; i < size_bin_true; i++)
    {
        if (input_bin[i] != true_bin[i])
        {
            free(input_bin);
            free(true_bin);
            return 0;
        }
    }
    free(input_bin);
    free(true_bin);

    return 1;
}








