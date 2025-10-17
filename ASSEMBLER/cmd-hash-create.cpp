#include <stdio.h>

#include "../COMMON/comand.h"
#include "../COMMON/support.h"


int main(void) {
    FILE* output_stream = fopen_file(NAME_HASH_FILE, "w");

    fprintf(output_stream, "enum INT_HASH {\n");

    for (size_t i = 0; i < (sizeof(CHAR_CMD) / sizeof(CHAR_CMD[0])); i++) {
        fprintf(output_stream, "    HASH_%s = %d,\n", CHAR_CMD[i], cmd_to_hash(CHAR_CMD[i]));
    }

    fprintf(output_stream, "};");

    fclose_file(output_stream);

    return 0;
}
