#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree-type.h"
#include "tree-const.h"
#include "tree-func.h"

static int AMOUNT_IMAGES = 0;

// -------------------------------------------------------------------------------------------------------
int print_node_in (node_t* point)
{
    assert(point);

    if (point->left)
    {
        putchar ('(');
        print_node_in (point->left);
    }

    printf(FORMAT_TRV " ", point->value);

    if (point->right)
    {
        putchar ('(');
        print_node_in (point->right);
    }

    putchar (')');

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int dump_tree (tree_t* tree)
{
    assert (tree);

    create_graph (tree, ADDRESS_DOT);

    char comand[200] = {};
    sprintf (comand,
             "dot %s -T png -o %s%d.png",
             ADDRESS_DOT,
             ADDRESS_IMG,
             AMOUNT_IMAGES);

    int trash = system (comand);
    (void) trash;

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_graph (tree_t* tree,
                  const char* name_file)
{
    assert (tree);

    FILE* file = fopen (name_file, "w");
    assert (file);

    fprintf (file,
            "digraph {\n"
            "  rankdir=UD;\n"
            "  bgcolor=\"#1e1e1e\""
            // "  splines=ortho;\n"
            "  nodesep=0.4;\n"
            "  ranksep=0.6;\n"
            "  node [shape=Mrecord, style=filled, fontname=\"Helvetica\"];\n"
            "  edge [arrowhead=vee, arrowsize=0.6, penwidth=1.2];\n\n");

    create_block (file, tree->root, 0);
    create_lines (file, tree->root);

    fprintf(file, "}\n");
    fclose (file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_block (FILE* file,
                  node_t* root,
                  int rank)
{
    assert(root);

    if (root->left)
    {
        create_block (file, root->left, rank + 1);
    }

    const char* color = NULL;

    if (rank == 0)
    {
        color = "#aa00deff";
    }

    else
    {
        color = "#2c8deeff";
    }

    fprintf(file,
            "block_%p [label=\"{%p|VALUE=" FORMAT_TRV "|BIRD=%c|{LEFT=%p|RIGHT=%p}}\", fillcolor=\"%s\"];\n",
            root, root, root->value, root->bird, root->left, root->right, color);

    if (root->right)
    {
        create_block (file, root->right, rank + 1);

    }

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_lines (FILE* file,
                  node_t* root)
{
    assert (file);
    assert (root);

    if (root->left)
    {
        create_lines (file, root->left);
    }

    if (root->left != NULL)
    {
        fprintf (file,
             "block_%p -> block_%p [color=\"#ffea05ff\", penwidth = 1.5, arrowsize = 0.6, constraint = true];\n",
             root,
             root->left);
    }

    if (root->right != NULL)
    {
    fprintf (file,
             "block_%p -> block_%p [color=\"#de05ffff\", penwidth = 1.5, arrowsize = 0.6, constraint = true];\n",
             root,
             root->right);
    }

    if (root->right)
    {
        create_lines (file, root->right);
    }

    return 1;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
// int create_html (



