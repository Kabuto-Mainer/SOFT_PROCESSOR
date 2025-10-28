#ifndef LIST_CONST_H
#define LIST_CONST_H

// ------------------------------------------------------------------------------------------------------
const char DISC_ERROR[][100] =
{
    "NOT_ERRORS",                       // 0
    "CALLOC RETURN NULL",               // 1
    "ADDRESS DATA = NULL",              // 2
    "ADDRESS INDEX_INF = NULL",         // 4
    "VALUE FREE INDEX != L_FREE_NUM",   // 8
    "TAIL BIGGER SIZE",                 // 16
    "HEAD BIGGER SIZE",                 // 32
    "ORDER NEXT INDEX IS NOT TRUE",     // 64
    "ORDER PREV INDEX IS NOT TRUE",     // 128
    "ORDER FREE INDEX IS NOT TRUE",     // 256
    "NEXT OF TAIL IS NOT = 0",          // 512
    "PREV OF HEAD IS NOT = 0",          // 1024
    "ERROR IN VERIFIER",                // 2048
    "INDEX  TO INSERT IS NOT CORRECT",  // 4096
    "INDEX TO DELETE IS NOT CORRECT",   // 8192
    "NOT ENOUGH SIZE TO PUSH ELEMENT",  // 16384
    "TOO BIG INDEX IN INDEX_INF"        // 32768
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
