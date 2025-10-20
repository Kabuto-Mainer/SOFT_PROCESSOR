#include <stdio.h>
#include <assert.h>
#include <cstdlib>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "../COMMON/support.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"


//------------------------------------------------------------------------------------------------
int main(int argv, char* args[])
{
    label_t table_label[AMOUNT_POINTS] = {};

    for (int i = 0; i < AMOUNT_POINTS; i++)
    {
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

    if (table_label[0].name[0] != '\0')
    {
        my_assembler(name_input_file, name_output_file, NAME_TEXT_FILE, table_label, 1, &disp_set);
    }
    return 0;
}
//------------------------------------------------------------------------------------------------

