#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEY_LEN 6
#define T 2
#define MAX_KEYS (2 * T - 1)
#define MAX_CHILDREN (2 * T)
// t = 2, узлы могут содержать от t-1 до 2t-1  ключей
typedef struct
{
    char key[MAX_KEY_LEN + 1];
    double value;
} Item;

typedef struct BTreeNode
{
    Item items[MAX_KEYS];
    struct BTreeNode *children[MAX_CHILDREN];
    int keyCount;
    bool leaf;
} BTreeNode;

void free_tree(BTreeNode *root);
void free_node(BTreeNode *node);

BTreeNode *btree_insert(BTreeNode *root, const char *key, double value);
BTreeNode *btree_delete(BTreeNode *root, const char *key);
void btree_print(BTreeNode *root);
Item *btree_search(BTreeNode *root, const char *key);

#endif
