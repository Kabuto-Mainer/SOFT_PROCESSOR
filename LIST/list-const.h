#ifndef LIST_CONST_H
#define LIST_CONST_H

// ------------------------------------------------------------------------------------------------------
const char DISC_ERROR[][100] =
{
    "NOT_ERRORS",                       // 0x000000
    "CALLOC RETURN NULL",               // 0x000001
    "ADDRESS DATA = NULL",              // 0x000002
    "REALLOC RETURN NULL",              // 0x000004
    "ADDRESS INDEX_INF = NULL",         // 0x000008
    "FREE INDEX IS NOT FREE",           // 0x000010
    "ORDER NEXT INDEX IS NOT TRUE",     // 0x000020
    "ORDER FREE INDEX IS NOT TRUE",     // 0x000040
    "INFINITE CIRCLE IN VERIFIER",      // 0x000080
    "INSERT WITH FULL LIST" ,           // 0x000100
    "DELETE WITH ZERO LIST",            // 0x000200
    "ERROR IN VERIFIER",                // 0x000400
    "INDEX TO INSERT IS NOT CORRECT",   // 0x000800
    "INDEX TO DELETE IS NOT CORRECT",   // 0x001000
    "TOO BIG INDEX IN INDEX_INF",       // 0x002000
    "TOO BIG INDEX IN FREE",            // 0x004000
    "MIXING FREE AND DATA INDEX",       // 0x008000
    "CURRENT SIZE BIGGER CAPACITY",     // 0x010000
    "ERROR",                            // 0x020000
};
// ------------------------------------------------------------------------------------------------------
const char CHAR_BASE[] = "BASE";
const char CHAR_BEFORE[] = "BEFORE";
const char CHAR_AFTER[] = "AFTER";
const char CHAR_DELETE_A[] = "DELETE AFTER";
const char CHAR_DELETE_C[] = "DELETE CURRENT";
const char CHAR_INSERT[] = "INSERT";
const char CHAR_APPEND[] = "APPEND";
// ------------------------------------------------------------------------------------------------------
const char CHAR_FUNC_INSERT_AFTER[] = "list_insert_after";
const char CHAR_FUNC_APPEND[] = "list_append";
const char CHAR_FUNC_DELETE_CURRENT[] = "list_delete_current";
const char CHAR_FUNC_DELETE_AFTER[] = "list_delete_after";
const char CHAR_FUNC_PRINT[] = "list_print";
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
#define BLUE_COLOR   "<mark style=\"background-color: #1808adff; color: #e0e0e0\">"
#define GREEN_COLOR  "<mark style=\"background-color: #3aaa06ff; color: #e0e0e0\">"
#define RED_COLOR    "<mark style=\"background-color: #cc0505ff; color: #e0e0e0\">"
#define PURPLE_COLOR "<mark style=\"background-color: #9509b8ff; color: #e0e0e0\">"
#define GREY_COLOR   "<mark style=\"background-color: #747474ff; color: #e0e0e0\">"
#define YELLOW_COLOR "<mark style=\"background-color: #ad9112ff; color: #e0e0e0\">"


#endif // LIST_CONST_H
