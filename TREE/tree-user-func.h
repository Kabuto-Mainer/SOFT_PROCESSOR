#ifndef TREE_USER_FUNC_H
#define TREE_USER_FUNC_H

#include "tree-type.h"
#include "tree-func.h"

// -------------------------------------------------------------------------------------------------------
int def_createTree (tree_t* tree,
                    const char* file,
                    int number_line,
                    const char* name);
// -------------------------------------------------------------------------------------------------------
int def_deleteTree (tree_t* tree);
// -------------------------------------------------------------------------------------------------------
node_t* def_addValue (tree_t* tree,
                      trv_t value,
                      const char* file,
                      int number_line);
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
#define createTree(tree) def_createTree (tree, __FILE__, __LINE__, #tree)
#define deleteTree(tree) def_deleteTree (tree)
#define addValue(tree, value) def_addValue (tree, value, __FILE__, __LINE__)
// -------------------------------------------------------------------------------------------------------


#endif // TREE_USER_FUNC_H
