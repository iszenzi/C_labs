#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

node *create_node(int data)
{
    node *newnode = malloc(sizeof(node));
    if (!newnode)
    {
        printf("ERROR: Memory allocation\n");
        exit(1);
    }

    newnode->data = data;
    newnode->left = NULL;
    newnode->right = NULL;

    return newnode;
}

node *insert_node(node *root, int data)
{
    if (!root)
    {
        return create_node(data);
    }
    if (data < root->data)
    {
        root->left = insert_node(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = insert_node(root->right, data);
    }
    return root;
}

void tree_free(node *root)
{
    if (root)
    {
        tree_free(root->left);
        tree_free(root->right);
        free(root);
    }
}