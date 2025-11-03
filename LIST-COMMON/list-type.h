#ifndef LIST_TYPE_H
#define LIST_TYPE_H

#include <stdio.h>

// All supported errors in this program
// -------------------------------------------------------------------------------------------------------
enum LIST_ERRORS
{
    L_NOT_ERRORS    = 0x0000,
    L_NULL_FICT     = 0x0001,
    L_NULL_PREV     = 0x0002,
    L_NULL_NEXT     = 0x0004,
    L_NULL_EL       = 0x0008,
    L_BAD_ORDER     = 0x0010,
    L_BAD_SIZE      = 0x0020,
    L_NULL_CALLOC   = 0x0040,
    L_NULL_INPUT    = 0x0080
};
// -------------------------------------------------------------------------------------------------------
const int MAX_ERROR = 0x0080;
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
typedef int lsd_t; // Type of data in list
// -------------------------------------------------------------------------------------------------------
#define FORMAT_DATA "%d"
#define DUMP_FORMAT_DATA "%8d"
// -------------------------------------------------------------------------------------------------------
struct list_node_t    // Struct one element of list
{
    lsd_t data;
    list_node_t* prev;
    list_node_t* next;
};
// -------------------------------------------------------------------------------------------------------
struct list_error_inf // Struct with all information of errors with list
{
    const char* name_file;
    int number_line;
    const char* name_func;
    int error;
};
// -------------------------------------------------------------------------------------------------------
struct list_create_inf // Struct with all information about creating list
{
    const char* name_file;
    int number_line;
    const char* name_list;
    long long time_create;
};
//-------------------------------------------------------------------------------------------------
struct list_t // Main struct of list
{
    list_node_t* fict;
    size_t size;
    list_error_inf error_inf;
    list_create_inf create_inf;
};
// -------------------------------------------------------------------------------------------------------
















#endif
