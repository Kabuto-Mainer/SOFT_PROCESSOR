#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "processor.h"
#include "display.h"
#include "../COMMON/comand.h"
#include "../COMMON/config.h"
#include "../COMMON/support.h"


#define WIDTH  400
#define HEIGHT 300


int main(int argv, char* args[]) {

    if (argv > 1) {
        FILE* buffer = fopen_file(args[1], "wb");
        fclose_file(buffer);

        printf(_G_ "Using %s file\n" _N_, args[1]);
        my_proc(args[1]);
    }

    else {
        my_proc(NAME_BIN_FILE);
    }

    return 0;
}
