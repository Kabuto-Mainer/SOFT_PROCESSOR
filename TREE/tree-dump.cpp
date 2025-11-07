#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree-type.h"
#include "tree-config.h"
#include "tree-const.h"
#include "tree-func.h"
#include "PARSER/comand.h"
// #include "PARSER/hash-cmd.h"

extern int MASS_HASH[];

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
int dump_tree (tree_t* tree,
               const char* reason)
{
    assert (tree);

    char address[200] = {};
    sprintf (address,
             "%s_%d/graph.dot",
             ADDRESS_DUMP_DIR,
             tree->create.number_dir);

    create_graph (tree, address);

    char comand[200] = {};
    sprintf (comand,
             "dot %s -T png -o %s_%d/IMAGES/img%d.png",
             address,
             ADDRESS_DUMP_DIR,
             tree->create.number_dir,
             AMOUNT_IMAGES);

    int trash = system (comand);
    (void) trash;

    sprintf (address,
             "%s_%d/dump.html",
             ADDRESS_DUMP_DIR,
             tree->create.number_dir);

    create_html (tree, address, reason);
    AMOUNT_IMAGES++;

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
            "  splines=ortho;\n"
            "  nodesep=0.4;\n"
            "  ranksep=0.6;\n"
            "  node [shape=plaintext, style=filled, fontname=\"Helvetica\"];\n"
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

    fprintf (file,
             "block_%p [label=<\n<TABLE CELLSPACING=\"0\" CELLPADDING=\"4\">\n"
             "<TR><TD PORT=\"root\" BGCOLOR=\"#0308f9ff\" COLSPAN=\"2\"><B>%p</B></TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\">VALUE</TD><TD BGCOLOR=\"#37ff05ff\">" FORMAT_TRV "</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\">AMOUNT</TD><TD BGCOLOR=\"#37ff05ff\">%d</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\">BIRD</TD><TD BGCOLOR=\"#37ff05ff\">%c</TD></TR>\n"
             "<TR><TD BGCOLOR=\"#f46b8bff\">ROOT</TD><TD BGCOLOR=\"#05fff7ff\">%p</TD></TR>\n"
             "<TR>\n<TD PORT=\"left\" BGCOLOR=\"#e8e804ff\">%p</TD>\n"
             "<TD PORT=\"right\" BGCOLOR=\"#e501faff\">%p</TD>\n</TR>\n</TABLE> >];\n\n",
             root, root, root->value, root->amount, root->bird, root->root, root->left, root->right);

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
        if (root->left->root == root)
        {
            fprintf (file,
                    "block_%p:left -> block_%p:root [color=\"#ffea05ff\", penwidth = 2, arrowsize = 0.6, constraint = true, dir = both];\n",
                    root, root->left);
        }

        else
        {
            fprintf (file,
                    "block_%p:left -> block_%p:root [color=\"#ff0505ff\", penwidth = 2, arrowsize = 0.6, constraint = true];\n",
                    root, root->left);
        }

        create_lines (file, root->left);
    }

    if (root->right)
    {
        if (root->right->root == root)
        {
            fprintf (file,
                 "block_%p:right -> block_%p:root [color=\"#e501faff\", penwidth = 2, arrowsize = 0.6, constraint = true, dir = both];\n",
                 root, root->right);
        }

        else
        {
            fprintf (file,
                 "block_%p:right -> block_%p:root [color=\"#fa0101ff\", penwidth = 2, arrowsize = 0.6, constraint = true];\n",
                 root, root->right);
        }

        create_lines (file, root->right);
    }

    return 1;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int clean_html (tree_t* tree)
{
    assert (tree);
    char address[200] = {};
    sprintf (address,
             "%s_%d/dump.html",
             ADDRESS_DUMP_DIR,
             tree->create.number_dir);

    FILE* buffer = fopen (address, "w");
    fprintf (buffer,
             "<pre style=\"font-family: 'Courier New', monospace; font-size: 14px; color: #e0e0e0; background-color:"
             " #1e1e1e; padding: 10px; border-radius: 6px;\">\n\n");
    fclose (buffer);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int create_html (tree_t* tree,
                 const char* name_file,
                 const char* reason)
{
    assert (tree);
    assert (name_file);

    FILE* file = fopen (name_file, "a");
    if (file == NULL)
    {
        printf ("ERROR: NULL file in create_html\n");
        return -1;
    }

    fprintf (file,
             "// ------------------------------------------------------------------------------------------------------\n");
    fprintf (file,
             "Dump Tree\nTree \"%s\" had created in \"%s:%d\"\n",
             tree->create.name,
             tree->create.file,
             tree->create.line);
    fprintf (file,
             "Called in \"%s:%d\" in function \"%s\"\n",
             tree->error.file,
             tree->error.line,
             tree->error.func);
    fprintf (file,
             "Reason dump: %s",
             reason);

    if (tree->error.error == NOT_ERRORS)
    {
        fprintf (file,
                 "Not errors in tree\n");
    }

    else
    {
        fprintf (file,
                 "Errors for tree:\n");

            int error = 1;
        for (int counter = 1;
             (error << counter) < MAX_ERROR; counter++)
        {
            if ((tree->error.error & (error << counter)) == (error << counter))
            {
                fprintf (file,
                         "    \"%s\"\n",
                         DESC_ERROR[counter]);
            }
        }
    }

    fprintf (file,
             "Tree size: [%zu]\n",
             tree->size);

    size_t size_img = 0;
    if ((tree->error.error & BAD_SIZE) != BAD_SIZE)
    {
        size_img = tree->size * STANDARD_SIZE_BLOCK;
    }

    else
    {
        size_img = STANDARD_SIZE_IMG;
    }

    fprintf (file,
             "<img src=%s_%d/IMAGES/img%d.png width=%zupx>\n",
             ADDRESS_DUMP_DIR,
             tree->create.number_dir,
             AMOUNT_IMAGES,
             size_img);

    fclose (file);

    return 0;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int clean_dump_dir (const int number_dir)
{
    char address[200] = {};
    sprintf (address,
            "%s_%d/IMAGES/trash.txt",
             ADDRESS_DUMP_DIR,
             number_dir);

    FILE* buffer = fopen (address, "w");
    fprintf (buffer, "This file deletes warning of system comand\n");
    fclose (buffer);

    char comand[200] = {};
    sprintf (comand,
             "rm -r %s_%d/IMAGES/*",
             ADDRESS_DUMP_DIR,
             number_dir);

    int ret_value = system (comand);
    return ret_value;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int find_dir (const size_t code)
{
    int counter = 1;
    for (size_t i = ((1 << 4) - 1); i <= (((size_t) (1 << 4) - 1) << 4 * 7); i <<= 4)
    {
        if ((code & i) == 0)
        {
            return counter;
        }

        counter++;
    }

    return -1;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
size_t find_cmd (comand_inf_t* comand_inf,
                 int hash,
                 int number)
{
    assert (comand_inf);
    assert (comand_inf->mass_cmd);

    int counter = 0;
    for (size_t i = 0; i < comand_inf->amount_cmd; i++)
    {
        if (comand_inf->mass_cmd[i].hash == hash)
        {
            counter++;

            if (counter == number)
            {
                return comand_inf->mass_cmd[i].arg;
            }
        }
    }

    return (size_t) -1;
}
// -------------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------------
int rewrite_dump_dir (tree_t* tree)
{
    assert (tree);

    size_t mask_code = (1 << 4) - 1;
    size_t code_dir = find_cmd (tree->create.cmd_inf,
                                MASS_HASH[DEL_DUMP],
                                1);
    size_t new_code = 0;
    for (size_t counter = 0; counter < 8; counter++)
    {
        size_t buf_code = code_dir & (mask_code << (counter * 4));
        printf ("BUF CODE: %0lx\nMINUS: %0x\n", buf_code, (unsigned) (2 << (counter * 4)));
        buf_code = (buf_code - ((size_t) 2 << (counter * 4))) & (mask_code << (counter * 4));
        printf ("CODE: %08lx\n", buf_code);
        printf ("MASK: %08lx\n", mask_code << (counter * 4));
        new_code |= buf_code;
    }

    printf ("OLD = %0lx\nNEW =%0lx\"",
            code_dir,
            new_code);

    return 0;
}
// -------------------------------------------------------------------------------------------------------
