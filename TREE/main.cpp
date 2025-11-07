#include <stdio.h>

#include "tree-func.h"
#include "tree-user-func.h"

int main(void)
{
    tree_t tree = {};

    createTree (&tree);
    addValue (&tree, 10);
    addValue (&tree, 5);
    addValue (&tree, 15);
    addValue (&tree, 13);
    addValue (&tree, 17);
    addValue (&tree, 3);
    addValue (&tree, 7);

    deleteTree (&tree);

    return 0;
}
