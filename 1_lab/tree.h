#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *left;
    struct node *right;
} node;

node *create_node(int data);

node *insert_node(node *root, int data);

void tree_free(node *root);

void tree_print(node *root, int depth);

node *deleate_node(node *root, int data);

node *find_min_node(node *root);

#endif
