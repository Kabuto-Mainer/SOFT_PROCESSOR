#ifndef LIST_TYPE_H
#define LIST_TYPE_H

#include <math.h>

enum list_error_t
{
    L_NOT_ERRORS        = 0x000000,
    L_CALLOC_NULL       = 0x000001,
    L_REALLOC_NULL      = 0x000002,
    L_NULL_DATA         = 0x000004,
    L_NULL_INDEX        = 0x000008,
    L_FREE_NOT_FREE     = 0x000010,
    L_BAD_INDEX_ORDER   = 0x000020,
    L_BAD_FREE_ORDER    = 0x000040,
    L_INFINITE_FREE     = 0x000080,
    L_INSERT_SIZE       = 0x000100,
    L_DELETE_SIZE       = 0x000200,
    L_ERROR_VER         = 0x000400,
    L_INSERT_INDEX      = 0x000800,
    L_BAD_DEL           = 0x001000,
    L_BIG_INDEX         = 0x002000,
    L_BIG_FREE          = 0x004000,
    L_MIX_INDEX_FREE    = 0x008000,
    L_SIZE_B_CAPACITY   = 0x010000,
    L_ERROR             = 0x020000
};
// -------------------------------------------------------------------------------------------------------
// Необходимо именно тут для удобства
const int MAX_ERROR = 0x008000;
// -------------------------------------------------------------------------------------------------------
#define FORMAT_ERROR "%06X"
// -------------------------------------------------------------------------------------------------------
enum return_value
{
    R_OK = 0,
    R_ERR = -1,
};
// -------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------
typedef double lsd_t;
typedef int lsi_t;
// -------------------------------------------------------------------------------------------------------
#define FORMAT_LSD_T "%lg"
#define FORMAT_LSI_T "%d"
#define DUMP_FORMAT_LSD_T "%8.2f"
#define DUMP_FORMAT_LSI_T "%8d"
// -------------------------------------------------------------------------------------------------------
const lsd_t L_FREE_NUM = 0xDEDE;
// -------------------------------------------------------------------------------------------------------
const lsd_t L_ERR_VALUE = NAN;
// -------------------------------------------------------------------------------------------------------
struct index_t
{
    lsi_t next;
    lsi_t prev;
};
// -------------------------------------------------------------------------------------------------------
struct list_create_inf_t
{
    const char* name_file;
    const char* name_list;
    int number_line;
    size_t start_size;
    long long time_start;
};
// -------------------------------------------------------------------------------------------------------
struct list_error_inf_t
{
    int current_error;
    const char* name_call_func;
    const char* name_call_file;
    int number_call_line;
};
// -------------------------------------------------------------------------------------------------------
struct list_inf_t
{
    list_create_inf_t create_inf;
    list_error_inf_t error_inf;
    size_t current_size;
    size_t capacity;
};
// -------------------------------------------------------------------------------------------------------
struct list_t
{
    lsd_t* data;
    lsi_t free;

    index_t* index_inf;
    list_inf_t list_inf;
};
// -------------------------------------------------------------------------------------------------------



#endif // LIST_TYPE_H
