#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree-type.h"
#include "tree-const.h"
#include "tree-func.h"

int create_tree (tree_t* tree)
{
    assert (tree);

    node_t* buffer = (node_t*) calloc (1, sizeof(node_t));
    if (buffer == NULL)
    {
        return -1;
    }

    buffer->left = NULL;
    buffer->right = NULL;
    buffer->bird = SIGN_FICT;
    buffer->value = FREE_VALUE;

    tree->root = buffer;
    tree->size = 1;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
node_t* create_node (node_t* root, trv_t value, sides side)
{
    node_t* buffer = (node_t*) calloc (1, sizeof(node_t));
    if (buffer == NULL)
    {
        return NULL;
    }

    if (side == LEFT)
    {
        root->left = buffer;
    }

    else
    {
        root->right = buffer;
    }

    buffer->value = value;
    buffer->left = NULL;
    buffer->right = NULL;
    buffer->bird = SIGN_COMMON;

    return buffer;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int delete_node (node_t* root)
{
    assert(root);

    if (root->left)
    {
        delete_node (root->left);
    }

    if (root->right)
    {
        delete_node (root->right);
    }

    free (root);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// // -------------------------------------------------------------------------------------------------------
// int create_dump_dir (const char* name_file)
// {
//     assert (name_file);

//     FILE* config = fopen (name_file, "r");
//     if (config == NULL)
//     {
//         printf("ERROR: NULL file in create_dump_dir\n");
//         return -1;
//     }
//     return 0;

// }



