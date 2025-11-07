#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "parser.h"
#include "comand.h"
extern int MASS_HASH[];
extern int HASH_DEL_DUMP;
// #include "hash-cmd.h"


// TODO Don't know, what can I do with it
// -------------------------------------------------------------------------------------------------------
comand_inf_t* parser_config (const char* name_file)
{
    assert (name_file);

    file_data_t data = {};

    make_mass (name_file, &data);
    fill_comand (&data);

    comand_t* mass_cmd = (comand_t*) calloc (data.amount_cmd, sizeof (comand_t));
    if (mass_cmd == NULL)
    {
        delete_data (&data);
        printf ("ERROR: NULL calloc inparser_config\n");
        return NULL;
    }

    comand_inf_t* comand_inf = (comand_inf_t*) calloc (1, sizeof (comand_inf_t));
    if (comand_inf == NULL)
    {
        delete_data (&data);
        free (mass_cmd);
        printf ("ERROR: NULL calloc inparser_config\n");
        return NULL;
    }

    for (size_t i = 0; i < data.amount_cmd; i++)
    {
        mass_cmd[i] = data.comands[i];
    }

    comand_inf->mass_cmd = mass_cmd;
    comand_inf->amount_cmd = data.amount_cmd;

    delete_data (&data);

    return comand_inf;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int make_mass (const char* name,
               file_data_t* data)
{
    assert (name);

    size_t size = file_size_check (name);
    char* buffer = (char*) calloc (size + 1, sizeof (char));
    if (buffer == NULL)
    {
        printf ("ERROR: NULL calloc in make_mass");
        return -1;
    }

    FILE* file = fopen (name, "r");
    if (file == NULL)
    {
        printf ("ERROR: NULL file in make_mass\n");
        free (buffer);
        return -1;
    }
    size_t amount_char = fread (buffer, sizeof (char), size, file);
    fclose (file);

    buffer[size] = '\0';

    for (size_t i = 0; i < amount_char; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            data->amount_line++;
        }

        else if (buffer[i] == '$')
        {
            data->amount_cmd++;
        }

        else if (buffer[i] == '\0')
        {
            break;
        }
    }

    data->amount_line++;

    char** mass_point= (char**) calloc (data->amount_line, sizeof (char*));
    if (mass_point == NULL)
    {
        printf ("ERROR: NULL calloc in make_mass\n");
        free (buffer);
        return -1;
    }
    comand_t* mass_cmd = (comand_t*) calloc (data->amount_cmd, sizeof (comand_t));
    if (mass_cmd == NULL)
    {
        printf ("ERROR: NULL calloc in make_mass\n");
        free (buffer);
        free (mass_point);
        return -1;
    }

    data->buffer = buffer;
    data->mass_point = mass_point;
    data->comands = mass_cmd;

    filling_mass_point (data);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
size_t fill_comand (file_data_t* data)
{
    assert (data);
    assert (data->buffer);
    assert (data->mass_point);


    for (size_t i = 0; i < data->amount_line; i++)
    {
        // printf ("%s\n", data->mass_point[i]);
        if (data->mass_point[i][0] == '$')
        {
            size_t arg = 0;
            char comand[200] = {};

            sscanf (data->mass_point[i] + 1, "%[^:$ ] %zu", comand, &arg);
            // printf ("CMD: %s\n", comand);
            int hash = make_hash (comand);
            // printf ("HASH: %d\n", hash);

            char flag = 0;
            for (size_t index = 0; index < sizeof (CHAR_COMANDS) / sizeof (CHAR_COMANDS[0]); index++)
            {
                // printf ("HASH_2: %d\n", MASS_HASH[index]);
                if (hash == MASS_HASH[index])
                {
                    data->comands[index].hash = hash;
                    data->comands[index].arg = arg;
                    flag = 1;
                    break;
                }
            }

            if (flag == 0)
            {
                printf ("ERROR: unknown comand in:\n-%s-\n", data->mass_point[i]);
                return 0;
            }
        }
    }

    return 1;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int filling_mass_point (file_data_t* data)
{
    assert (data);

    char* point = data->buffer;
    for (size_t i = 0; i < data->amount_line; i++)
    {
        data->mass_point[i] = point;
        point = find_char (point, '\0') + 1;
        // printf ("%s\n", point);
    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int delete_data (file_data_t* data)
{
    assert (data);

    free (data->buffer);
    free (data->comands);
    free (data->mass_point);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
char* find_char (char* string,
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
// -------------------------------------------------------------------------------------------------------

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






// // -------------------------------------------------------------------------------------------------------
// int create_dump_dir (const char* name_file)
// {
//     assert (name_file);

//     FILE* config = fopen (name_file, "r");
//     if (config == NULL)
//     {
//         printf("ERROR: NULL file in create_dump_dir\n");
//         return -1;
//     }
//     return 0;

// }
