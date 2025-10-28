#ifndef LIST_TYPE_H
#define LIST_TYPE_H


enum list_error_t
{
    L_NOT_ERRORS        = 0,
    L_CALLOC_NULL       = 1,
    L_NULL_DATA         = 2,
    L_NULL_INDEX        = 4,
    L_BAD_FREE          = 8,
    L_BIG_TAIL          = 16,
    L_BIG_HEAD          = 32,
    L_BAD_INDEX_INF     = 64,
    L_BAD_INDEX_FREE    = 256,
    L_BAD_TAIL          = 512,
    L_BAD_HEAD          = 1024,
    L_ERROR_VER         = 2048,
    L_INSERT_INDEX      = 4096,
    L_BAD_DEL           = 8192,
    L_BAD_SIZE          = 16384,
    L_TOO_BIG_INDEX     = 32768,
    L_ERROR             = 65536
};
// -------------------------------------------------------------------------------------------------------
// Необходимо именно тут для удобства
const int MAX_ERROR = 32768;
// -------------------------------------------------------------------------------------------------------
enum list_func_t
{
    L_INSERT            = 0,
    L_DELETE_AFTER      = 1,
    L_DELETE_CURRENT    = 2,
};
// -------------------------------------------------------------------------------------------------------
typedef int lsd_t;
typedef int lsi_t;
// -------------------------------------------------------------------------------------------------------
const lsd_t L_FREE_NUM = -1;
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
    int number_line;
    const char* name_list;
    size_t start_size;
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
};
// -------------------------------------------------------------------------------------------------------
struct list_t
{
    lsd_t* data;
    lsi_t free;
    lsi_t tail;
    lsi_t head;
    index_t* index_inf;

    list_inf_t list_inf;
};
// -------------------------------------------------------------------------------------------------------
enum list_dump_t
{
    L_BASE_DUMP       = 0,
    L_INSERT_AFTER      = 1,
    L_INSERT_BEFORE     = 2,
    L_DELETE_A_AFTER    = 3,
    L_DELETE_A_BEFORE   = 4,
    L_DELETE_C_AFTER    = 5,
    L_DELETE_C_BEFORE   = 6,
    L_APPEND_AFTER      = 7,
    L_APPEND_BEFORE     = 8
};



#endif // LIST_TYPE_H
