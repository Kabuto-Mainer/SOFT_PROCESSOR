#ifndef MY_LIST_H
#define MY_LIST_H

#include "list-config.h"
#include "list-const.h"
#include "list-func.h"
#include "list-type.h"




// -------------------------------------------------------------------------------------------------------
#define insertAfter(list, index, value) def_insertAfter(list, index, value, "insertAfter", __FILE__, __LINE__)
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
#define deleteCurrent(list, index) def_deleteCurrent(list, index, "deleteCurrent", __FILE__, __LINE__)
#define deleteAfter(list, index) def_deleteAfter(list, index, "deleteAfter", __FILE__, __LINE__)
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
#define printList(list) def_printList(list, "printList", __FILE__, __LINE__)
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
#define sortList(list) def_sortList(list, "sortList", __FILE__, __LINE__)
// -------------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------------
#define getTail(list) list_tail(list)
#define getHead(list) list_head(list)
#define getError(list) list_error(list)
#define getNext(list, index) list_next(list, index)
#define getPrev(list, index) list_prev(list, prev)
#define getValue(list, index) lis_value(list, index)
// -------------------------------------------------------------------------------------------------------






#endif
