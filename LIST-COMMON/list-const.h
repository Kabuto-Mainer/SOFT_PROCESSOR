#ifndef LIST_CONST_H
#define LIST_CONST_H

// DESCRIPTION ERRORS
//--------------------------------------------------------------------------------------------------------------------
const char DESC_ERROR[][100] =
{
    "NOT ERRORS WITH LIST",                 // 0x0000
    "NULL FICTIVE ELEMENT",                 // 0x0001
    "NULL PREV ELEMENT",                    // 0x0002
    "NULL NEXT ELEMENT",                    // 0x0004
    "NULL INDEX IN ELEMENT OF LIST",        // 0x0008
    "ORDER OF ELEMENT IS NOT CORRECT",      // 0x0010
    "CALL DELETE WITH ZERO SIZE",           // 0x0020
    "CALLOC RETURN NULL",                   // 0x0040
    "NULL ADDRESS HAD ENTERED TO FUNCTION"  // 0x0080
};
//--------------------------------------------------------------------------------------------------------------------

// ADDRESSES
//--------------------------------------------------------------------------------------------------------------------
const char NAME_DOT_FILE[]  = "LIST-COMMON/graph.dot"; // Address of dot file, where wrote each graph
const char NAME_HTML_FILE[] = "LIST-COMMON/dump.html"; // Address of html file, that you can open from your browser
const char ADDRESS_IMAGES[] = "LIST-COMMON/IMAGES";    // Address of all images graphs
const char ADDRESS_IMG_HTML[] = "IMAGES/";             // Address of images for html file
//--------------------------------------------------------------------------------------------------------------------

// COLOR
//--------------------------------------------------------------------------------------------------------------------
#define TRUE_EL     "#3e85c0ff"
#define NOT_TRUE_EL "#be6217ff"
#define NULL_EL     "#c42d2dff"
#define ALL_NULL_EL "#be20b7ff"
#define FICT_EL     "#39a025ff"
//--------------------------------------------------------------------------------------------------------------------

// HTML COLOR
//--------------------------------------------------------------------------------------------------------------------
#define BLUE_COLOR   "<mark style=\"background-color: #1808adff; color: #e0e0e0\">"
#define GREEN_COLOR  "<mark style=\"background-color: #3aaa06ff; color: #e0e0e0\">"
#define RED_COLOR    "<mark style=\"background-color: #cc0505ff; color: #e0e0e0\">"
#define PURPLE_COLOR "<mark style=\"background-color: #9509b8ff; color: #e0e0e0\">"
#define GREY_COLOR   "<mark style=\"background-color: #747474ff; color: #e0e0e0\">"
#define YELLOW_COLOR "<mark style=\"background-color: #ad9112ff; color: #e0e0e0\">"
//--------------------------------------------------------------------------------------------------------------------




#endif // LIST_CONST_H
