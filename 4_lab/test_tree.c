#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void test1_insert_basic()
{
    BTreeNode *root = NULL;
    root = btree_insert(root, "a", 1.0);
    root = btree_insert(root, "b", 2.0);
    root = btree_insert(root, "c", 3.0);

    assert(root != NULL);
    assert(root->keyCount == 3);
    assert(strcmp(root->items[0].key, "a") == 0);
    assert(strcmp(root->items[1].key, "b") == 0);
    assert(strcmp(root->items[2].key, "c") == 0);
    assert(root->leaf == true);

    free_tree(root);
}

void test2_split()
{
    BTreeNode *root = NULL;
    root = btree_insert(root, "a", 1.0);
    root = btree_insert(root, "b", 2.0);
    root = btree_insert(root, "c", 3.0);
    root = btree_insert(root, "d", 4.0);

    assert(root != NULL);
    // b станет корнем
    assert(root->keyCount == 1);
    assert(strcmp(root->items[0].key, "b") == 0);
    assert(root->leaf == false);

    assert(root->children[0]->keyCount == 1);
    assert(strcmp(root->children[0]->items[0].key, "a") == 0);

    assert(root->children[1]->keyCount == 2);
    assert(strcmp(root->children[1]->items[0].key, "c") == 0);
    assert(strcmp(root->children[1]->items[1].key, "d") == 0);

    free_tree(root);
}

void test3_search()
{
    BTreeNode *root = NULL;
    root = btree_insert(root, "x", 10.5);
    root = btree_insert(root, "y", 20.5);
    root = btree_insert(root, "z", 30.5);

    Item *res = btree_search(root, "y");
    assert(res != NULL);
    assert(res->value == 20.5);

    res = btree_search(root, "w");
    assert(res == NULL);

    free_tree(root);
}

void test4_merge()
{
    BTreeNode *root = NULL;

    // корень b левый a  правый c,d
    root = btree_insert(root, "a", 1.0);
    root = btree_insert(root, "b", 2.0);
    root = btree_insert(root, "c", 3.0);
    root = btree_insert(root, "d", 4.0);

    root = btree_delete(root, "d");

    root = btree_delete(root, "a");

    // rкорень b,c
    assert(root != NULL);
    assert(root->keyCount == 2);
    assert(strcmp(root->items[0].key, "b") == 0);
    assert(strcmp(root->items[1].key, "c") == 0);
    assert(root->leaf == true);

    free_tree(root);
}

void test5_large_tree()
{
    BTreeNode *root = NULL;
    char key[10];

    for (int i = 0; i < 25; i++)
    {
        sprintf(key, "k%02d", i);
        root = btree_insert(root, key, (double)i);
    }

    for (int i = 0; i < 25; i++)
    {
        sprintf(key, "k%02d", i);
        Item *res = btree_search(root, key);
        assert(res != NULL);
        assert(res->value == (double)i);
    }

    free_tree(root);
}

bool all_tests()
{
    test1_insert_basic();
    printf("Test 1 complete\n");
    test2_split();
    printf("Test 2 complete\n");
    test3_search();
    printf("Test 3 complete\n");
    test4_merge();
    printf("Test 4 complete\n");
    test5_large_tree();
    printf("Test 5 complete\n");
    return true;
}

int main()
{
    all_tests();
    return 0;
}
