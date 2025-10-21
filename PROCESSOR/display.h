#ifndef DISPLAY_H
#define DISPLAY_H

#include "processor.h"

int destroy_display(cpu_t* proc);

int update_display(cpu_t* proc);

int create_display(display_t* disp_set);


#endif
