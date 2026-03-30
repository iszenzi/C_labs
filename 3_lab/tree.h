#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    NODE_OP,
    NODE_VAR,
    NODE_NUM
} NodeType;

typedef struct expr_node
{
    NodeType type;

    union
    {
        char op;
        char var;
        int num;
    } data;

    struct expr_node *left;
    struct expr_node *right;
} expr_node;

expr_node *create_num_node(int num);
expr_node *create_var_node(char var);
expr_node *create_op_node(char op, expr_node *left, expr_node *right);
void free_tree(expr_node *root);
expr_node *parse_expression(const char *str);
void print_infix(expr_node *root);
void print_tree_visual(expr_node *root, int depth);
expr_node *reduce_multiplication(expr_node *root);

#endif
