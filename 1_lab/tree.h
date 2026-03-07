#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

node *delete_node(node *root, int data);

node *find_min_node(node *root);

bool check_range(node *root, int min, int max);

#endif
