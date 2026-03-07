#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tree.h"

void test1_insert_empty()
{
    node *root = NULL;
    root = insert_node(root, 27);
    assert(root != NULL);
    assert(root->data == 27);
    assert(root->left == NULL);
    assert(root->right == NULL);
    tree_free(root);
}

void test2_insert_left()
{
    node *root = NULL;
    root = insert_node(root, 14);
    root = insert_node(root, 12);
    assert(root != NULL);
    assert(root->data == 14);
    assert(root->left != NULL);
    assert(root->left->data == 12);
    assert(root->right == NULL);
    tree_free(root);
}

void test3_insert_right()
{
    node *root = NULL;
    root = insert_node(root, 42);
    root = insert_node(root, 52);
    assert(root != NULL);
    assert(root->data == 42);
    assert(root->left == NULL);
    assert(root->right != NULL);
    assert(root->right->data == 52);
    tree_free(root);
}

void test4_insert()
{
    node *root = NULL;
    root = insert_node(root, 100);
    root = insert_node(root, 10);
    root = insert_node(root, 110);
    assert(root != NULL);
    assert(root->data == 100);
    assert(root->left != NULL);
    assert(root->left->data == 10);
    assert(root->right != NULL);
    assert(root->right->data == 110);
    tree_free(root);
}

void test5_delete()
{
    node *root = NULL;
    root = insert_node(root, 50);
    root = insert_node(root, 30);
    root = insert_node(root, 70);
    root = delete_node(root, 30);
    assert(root != NULL);
    assert(root->data == 50);
    assert(root->left == NULL);
    assert(root->right != NULL);
    assert(root->right->data == 70);
    tree_free(root);
}

void test6_check_range()
{
    node *root = NULL;
    root = insert_node(root, 20);
    root = insert_node(root, 10);
    root = insert_node(root, 30);
    assert(check_range(root, 5, 35) == true);
    assert(check_range(root, 15, 25) == false);
    assert(check_range(root, 0, 15) == false);
    tree_free(root);
}

bool all_tests()
{
    test1_insert_empty();
    printf("Test 1 complete\n");
    test2_insert_left();
    printf("Test 2 complete\n");
    test3_insert_right();
    printf("Test 3 complete\n");
    test4_insert();
    printf("Test 4 complete\n");
    test5_delete();
    printf("Test 5 complete\n");
    test6_check_range();
    printf("Test 6 complete\n");
    return true;
}

int main()
{
    all_tests();
}