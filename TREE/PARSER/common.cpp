#include <stdio.h>
#include <assert.h>

#include "parser.h"

int make_hash (const char* string)
{
    assert (string);

    size_t index = 0;
    int hash = 5317;

    while (string[index] != '\0' )
    {
        hash += 33 * string[index];
        index++;
    }

    return hash;
}


