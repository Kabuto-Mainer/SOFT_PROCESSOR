#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include"../COMMON/config.h"


int color_pixel(int* frame, int x, int y, int color) {
    int address = (y - 1) * LEN_DISPLAY + x;

    frame[address] = color;


}





