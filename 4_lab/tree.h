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

typedef struct
{
    BTreeNode *root;
} BTree;

BTree *create_tree();
void free_tree(BTree *tree);
void free_node(BTreeNode *node);

void btree_insert(BTree *tree, const char *key, double value);
void btree_delete(BTree *tree, const char *key);
void btree_print(BTree *tree);
Item *btree_search(BTree *tree, const char *key);

#endif
