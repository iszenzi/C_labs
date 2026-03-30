#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "tree.h"

void test1_infix_to_rpn()
{
    char *rpn;

    rpn = infix_to_rpn("a * 3");
    assert(strcmp(rpn, "a 3 *") == 0);
    free(rpn);

    rpn = infix_to_rpn("(a + b) * 3");
    assert(strcmp(rpn, "a b + 3 *") == 0);
    free(rpn);

    rpn = infix_to_rpn("-a * (2 * 2 * 2)");
    assert(strcmp(rpn, "a ~ 2 2 * 2 * *") == 0);
    free(rpn);
}

void test2_build_ast()
{
    expr_node *root = parse_expression("a * 3");
    assert(root != NULL);
    assert(root->type == NODE_OP && root->data.op == '*');
    assert(root->left != NULL && root->left->type == NODE_VAR && root->left->data.var == 'a');
    assert(root->right != NULL && root->right->type == NODE_NUM && root->right->data.num == 3);
    free_tree(root);
}

void test3_reduce_simple()
{
    expr_node *root = parse_expression("a * 3");
    root = reduce_multiplication(root);

    assert(root->type == NODE_OP && root->data.op == '+');
    assert(root->right->type == NODE_VAR && root->right->data.var == 'a');
    assert(root->left->type == NODE_OP && root->left->data.op == '+');
    assert(root->left->left->type == NODE_VAR && root->left->left->data.var == 'a');

    free_tree(root);
}

void test4_reduce_complex()
{
    expr_node *root = parse_expression("(a + b) * 2");
    root = reduce_multiplication(root);

    assert(root->type == NODE_OP && root->data.op == '+');
    assert(root->left->type == NODE_OP && root->left->data.op == '+');
    assert(root->right->type == NODE_OP && root->right->data.op == '+');
    assert(root->left->left->type == NODE_VAR && root->left->left->data.var == 'a');

    free_tree(root);
}

void test5_reduce_constant_folding()
{
    expr_node *root = parse_expression("a * (2 * 2 * 2)");
    root = reduce_multiplication(root);

    assert(root->type == NODE_OP && root->data.op == '+');
    assert(root->right->type == NODE_VAR && root->right->data.var == 'a');

    free_tree(root);
}

void test6_evaluation()
{
    expr_node *root1 = parse_expression("2 + 3 * 4");
    assert(has_variables(root1) == false);
    assert(evaluate_tree(root1) == 14.0);
    free_tree(root1);

    expr_node *root2 = parse_expression("-(10 / 2) + 1");
    assert(has_variables(root2) == false);
    assert(evaluate_tree(root2) == -4.0);
    free_tree(root2);

    expr_node *root3 = parse_expression("a + 1");
    assert(has_variables(root3) == true);
    free_tree(root3);
}

bool all_tests()
{
    test1_infix_to_rpn();
    printf("Test 1 complete\n");
    test2_build_ast();
    printf("Test 2 complete\n");
    test3_reduce_simple();
    printf("Test 3 complete\n");
    test4_reduce_complex();
    printf("Test 4 complete\n");
    test5_reduce_constant_folding();
    printf("Test 5 complete\n");
    test6_evaluation();
    printf("Test 6 complete\n");
    return true;
}

int main()
{
    all_tests();
    return 0;
}