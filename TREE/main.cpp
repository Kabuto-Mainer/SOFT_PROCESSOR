#include <stdio.h>

#include "tree-func.h"

int main(void)
{
    tree_t tree = {};
    create_tree (&tree);

    create_node (tree.root, 10, LEFT);
    create_node (tree.root, 20, RIGHT);
    create_node (tree.root->left, 15, RIGHT);
    create_node (tree.root->left, 5, LEFT);

    dump_tree (&tree);

    print_node_in (tree.root);

    delete_node (tree.root);

    return 0;
}
