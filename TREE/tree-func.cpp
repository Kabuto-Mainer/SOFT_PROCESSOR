#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree-type.h"
#include "tree-const.h"
#include "tree-func.h"
#include "tree-config.h"
#include "PARSER/parser.h"
#include "PARSER/hash-cmd.h"
#include "PARSER/comand.h"

extern int MASS_HASH[];

// -------------------------------------------------------------------------------------------------------
int create_tree (tree_t* tree)
{
    assert (tree);

    node_t* buffer = (node_t*) calloc (1, sizeof(node_t));
    if (buffer == NULL)
    {
        return -1;
    }

    buffer->amount = 1;
    buffer->root = NULL;
    buffer->left = NULL;
    buffer->right = NULL;
    buffer->bird = SIGN_FICT;
    buffer->value = FREE_VALUE;

    comand_inf_t* comand_inf = parser_config (ADDRESS_CONFIG_DUMP);
    tree->create.cmd_inf = comand_inf;

    int number_dir = find_dir (find_cmd (comand_inf, MASS_HASH[DEL_DUMP], 1));
    // TODO add to config file --------\/-------
    if (number_dir < 1 || number_dir > 8)
    {
        printf ("ERROR: bad number dir\nUsing standard dir\n");
        number_dir = 1;
    }
    tree->create.number_dir = number_dir;
    clean_html (tree);
    // printf ("DIR: %d\n", number_dir);

    tree->root = buffer;
    tree->size = 1;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
node_t* create_node (node_t* root,
                     trv_t value,
                     sides side)
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

    buffer->root = root;
    buffer->amount = 1;
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

// -------------------------------------------------------------------------------------------------------
int delete_tree (tree_t* tree)
{
    assert (tree);

    // rewrite_dump_dir (tree);

    delete_node (tree->root);
    free (tree->create.cmd_inf->mass_cmd);
    free (tree->create.cmd_inf);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
node_t* add_elem (node_t* root,
                  trv_t value)
{
    assert (root);

    if (root->value == value)
    {
        root->amount++;
        return root;
    }

    else if (root->value > value)
    {
        if (root->left == NULL)
        {
            return create_node (root, value, LEFT);
        }

        else
        {
            return add_elem (root->left, value);
        }
    }

    else
    {
        if (root->right == NULL)
        {
            return create_node (root, value, RIGHT);
        }

        else
        {
            return add_elem (root->right, value);
        }
    }
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int check_node (node_t* root,
                size_t* counter)
{
    assert (root);

    // printf ("LEFT: %p\nRIGHT: %p\n", root->left, root->right);

    int ret_var = 0;
    if (root->left)
    {
        ret_var |= check_node (root->left, counter);
        // printf ("RET: %d\n", ret_var);

        if (root->left->root != root)
        {
            ret_var |= BAD_ROOT;
        }

        if (root->value <= root->left->value)
        {
            // printf ("2-%d : 1-%d\n", root->value, root->left->value);
            ret_var |= BAD_SORT;
        }
    }
    // printf ("RET_VALUE: %d\n", ret_var);

    if (root->bird != SIGN_COMMON && root->bird != SIGN_FICT)
    {
        ret_var |= BAD_SIGN;
    }

    if (root->amount < 1)
    {
        ret_var |= BAD_AMOUNT;
    }

    if (root->right)
    {
        ret_var |= check_node (root->right, counter);

        if (root->right->root != root)
        {
            ret_var |= BAD_ROOT;
        }

        if (root->value >= root->right->value)
        {
            // printf ("2-%d : 3-%d\n", root->value, root->right->value);
            ret_var |= BAD_SORT;
        }
    }

    (*counter)++;
    return ret_var;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int verifier (tree_t* tree)
{
    assert (tree);

    size_t size_tree = 0;

    int error = check_node (tree->root, &size_tree);
    if (size_tree != tree->size)
    {
        error |= BAD_SIZE;
    }

    tree->error.error = error;

    return error;
}
// -------------------------------------------------------------------------------------------------------
