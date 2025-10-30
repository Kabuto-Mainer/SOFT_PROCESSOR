#ifndef LIST_CONST_H
#define LIST_CONST_H

// ------------------------------------------------------------------------------------------------------
const char DISC_ERROR[][100] =
{
    "NOT_ERRORS",                       // 0x000000
    "ADDRESS DATA = NULL",              // 0x000002
    "CALLOC RETURN NULL",               // 0x000001
    "ADDRESS INDEX_INF = NULL",         // 0x000004
    "FREE INDEX IS NOT FREE",           // 0x000008
    "ORDER NEXT INDEX IS NOT TRUE",     // 0x000010
    "ORDER FREE INDEX IS NOT TRUE",     // 0x000020
    "INFINITE CIRCLE IN VERIFIER",      // 0x000040
    "INSERT WITH FULL LIST" ,           // 0x000080
    "DELETE WITH ZERO LIST",            // 0x000100
    "ERROR IN VERIFIER",                // 0x000200
    "INDEX TO INSERT IS NOT CORRECT",   // 0x000400
    "INDEX TO DELETE IS NOT CORRECT",   // 0x000800
    "TOO BIG INDEX IN INDEX_INF",       // 0x001000
    "TOO BIG INDEX IN FREE",            // 0x002000
    "MIXING FREE AND DATA INDEX",       // 0x004000
    "CURRENT SIZE BIGGER CAPACITY",     // 0x008000
    "ERROR",                            // 0x001000
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

#endif // LIST_CONST_H
