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


int main(int argv, char* args[])
{

    printf("ARG: %d\n", argv);
    if (argv > 1) {
        for (int i = 1; i < argv; i++) {
            printf("STRING: %s\n", args[i]);
            if (args[i][0] == '-' && args[i][1] == 'I') {
                printf("NAME_INPUT_FILE: %s\n", &(args[i][2]));
                my_proc(&(args[i][2]));
            }
        }
    }

    else {
        my_proc(NAME_BIN_FILE);
    }
//  my_proc(NAME_BIN_FILE);
    return 0;
}
