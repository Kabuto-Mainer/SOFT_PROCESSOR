#ifndef LIST_CONFIG_H
#define LIST_CONFIG_H

#define OFF 0
#define ON 1

// All check
// -------------------------------------------------------------------------------------------------------
#ifndef VERIFIER
#define VERIFIER ON
#endif
// -------------------------------------------------------------------------------------------------------

// Where we start this program
// -------------------------------------------------------------------------------------------------------
#ifndef GLOBAL
#define GLOBAL OFF
#endif
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
const int START_SIZE_LIST = 16;
// -------------------------------------------------------------------------------------------------------
// DO NOT CHANGE
const int MODE_REALLOC = 2;
// ------------------------------------------------------------------------------------------------------
const long long int TIME_DIV = 1e7;
// ------------------------------------------------------------------------------------------------------

// All addresses of files

#if GLOBAL == ON
// ------------------------------------------------------------------------------------------------------
const char NAME_LOG_FILE[] = "LIST/SUPPORT/list-log.log";
const char NAME_DUMP_FILE[] = "LIST/SUPPORT/list-dump.html";
const char ADDRESS_IMG_FILES[] = "LIST/IMAGES/img";
const char ADDRESS_IMG_HTML[] = "../IMAGES/img";
const char ADDRESS_IMAGES_DIR[] = "LIST/IMAGES";
const char NAME_DOT_FILE[] = "LIST/SUPPORT/graph.dot";
// ------------------------------------------------------------------------------------------------------
#else
// ------------------------------------------------------------------------------------------------------
const char NAME_LOG_FILE[] = "SUPPORT/list-log.log";
const char NAME_DUMP_FILE[] = "SUPPORT/list-dump.html";
const char ADDRESS_IMG_FILES[] = "IMAGES/img";
const char ADDRESS_IMG_HTML[] = "../IMAGES/img";
const char ADDRESS_IMAGES_DIR[] = "IMAGES";
const char NAME_DOT_FILE[] = "SUPPORT/graph.dot";
// ------------------------------------------------------------------------------------------------------
#endif // GLOBAL == ON



// ------------------------------------------------------------------------------------------------------
#define NAME(NAME_FILE) { #NAME_FILE }
// ------------------------------------------------------------------------------------------------------


#endif // LIST_CONFIG_H
