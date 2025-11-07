#include <stdio.h>
#include <assert.h>

#include "tree-func.h"
#include "tree-const.h"
#include "tree-type.h"
#include "tree-user-func.h"


// -------------------------------------------------------------------------------------------------------
int def_createTree (tree_t* tree,
                    const char* file,
                    int number_line,
                    const char* name)
{
    assert (tree);

    create_tree (tree);
    tree->create.file = file;
    tree->create.line = number_line;
    tree->create.name = name;

    tree->error.error = NOT_ERRORS;
    tree->error.file = NULL;
    tree->error.line = -1;
    tree->error.func = NULL;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int def_deleteTree (tree_t* tree)
{
    assert (tree);

    delete_tree (tree);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
node_t* def_addValue (tree_t* tree,
                      trv_t value,
                      const char* file,
                      int number_line)
{
    assert (tree);

    tree->error.file = file;
    tree->error.func = "addValue";
    tree->error.line = number_line;

    // printf ("ERROR: %d\n", tree->error.error);

    int error = verifier (tree);

    char reason[200] = {};
    if (error == NOT_ERRORS)
    {
        sprintf (reason,
                 "Dump before add [" FORMAT_TRV "]\n",
                 value);
        dump_tree (tree, reason);
        // printf ("eee\n");
    }

    else
    {
        sprintf (reason,
                 "ERROR before add [" FORMAT_TRV "]\n",
                 value);
        dump_tree (tree, reason);
        return NULL;
    }


    node_t* ret_value = add_elem (tree->root, value);
    tree->size++;

    error = verifier (tree);
    if (error == NOT_ERRORS)
    {
        sprintf (reason,
                 "Dump after add [" FORMAT_TRV "]\n",
                 value);
        dump_tree (tree, reason);
    }

    else
    {
        sprintf (reason,
                 "ERROR after add [" FORMAT_TRV "]\n",
                 value);
        dump_tree (tree, reason);
        return NULL;
    }

    return ret_value;
}
// -------------------------------------------------------------------------------------------------------
