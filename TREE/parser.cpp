#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>



int filling_mass_point (const char* string,
                        const char*



char* create_char_buffer (const size_t size)
{
    char* buffer = calloc (size, sizeof (char));
    if (buffer == NULL)
    {
        printf ("ERROR: NULL calloc in create_char_buffer");
    }
    return buffer;
}

const char* find_char (const char* string,
                       const char symbol)
{
    assert (string);

    size_t index = 0;
    while (1)
    {
        if (string[index] == symbol || string[index] == '\0')
        {
            return string + index;
        }
        index++;
    }

    return NULL;
}





    fread (buffer, sizeof (char), size, file);


// -------------------------------------------------------------------------------------------------------
size_t file_size_check(const char* name_file)
{
    assert(name_file);

    struct stat file_stat = {};

    if (stat(name_file, &file_stat) == -1)
    {
        printf("ERROR with stat\n");
        return 0;
    }

    return (size_t) file_stat.st_size;
}
// -------------------------------------------------------------------------------------------------------



