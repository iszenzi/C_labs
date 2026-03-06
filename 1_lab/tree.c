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

node *delete_node(node *root, int data)
{
    if (!root)
    {
        return NULL;
    }
    else if (data < root->data)
    {
        root->left = delete_node(root->left, data);
    }
    else if (data > root->data)
    {
        root->right = delete_node(root->right, data);
    }
    else
    {
        if (!root->left && !root->right)
        {
            free(root);
            return NULL;
        }
        else if (root->left != NULL && root->right != NULL)
        {
            node *min_node = find_min_node(root->right);
            root->data = min_node->data;
            root->right = delete_node(root->right, min_node->data);
        }
        else
        {
            node *a;
            if (root->left == NULL)
            {
                a = root->right;
            }
            else
            {
                a = root->left;
            }
            free(root);
            return a;
        }
    }
    return root;
}

node *find_min_node(node *root)
{
    int min = 10 ^ 10;
    while (root->left != NULL)
    {
        root = root->left;
    }
    return root->data;
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

void tree_print(node *root, int depth)
{
    if (!root)
    {
        return;
    }

    for (int i = 0; i < depth; i++)
    {
        printf("\t");
    }

    printf("%d\n", root->data);

    tree_print(root->left, depth + 1);
    tree_print(root->right, depth + 1);
}