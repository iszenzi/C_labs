#include "tree.h"
#include <ctype.h>
#include <string.h>

expr_node *create_num_node(int num)
{
    expr_node *node = (expr_node *)malloc(sizeof(expr_node));
    if (!node)
        return NULL;
    node->type = NODE_NUM;
    node->data.num = num;
    node->left = NULL;
    node->right = NULL;
    return node;
}

expr_node *create_var_node(char var)
{
    expr_node *node = (expr_node *)malloc(sizeof(expr_node));
    if (!node)
        return NULL;
    node->type = NODE_VAR;
    node->data.var = var;
    node->left = NULL;
    node->right = NULL;
    return node;
}

expr_node *create_op_node(char op, expr_node *left, expr_node *right)
{
    expr_node *node = (expr_node *)malloc(sizeof(expr_node));
    if (!node)
        return NULL;
    node->type = NODE_OP;
    node->data.op = op;
    node->left = left;
    node->right = right;
    return node;
}

void free_tree(expr_node *root)
{
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

static int get_precedence(char op)
{
    switch (op)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return 0;
    }
}

char *infix_to_rpn(const char *infix)
{
    char *rpn = (char *)malloc(1024);
    if (!rpn)
        return NULL;

    int r = 0;
    char op_stack[512];
    int top = -1;

    while (*infix)
    {
        if (isspace(*infix))
        {
            infix++;
            continue;
        }

        if (isdigit(*infix))
        {
            while (isdigit(*infix))
            {
                rpn[r++] = *infix++;
            }
            rpn[r++] = ' ';
        }
        else if (isalpha(*infix))
        {
            rpn[r++] = *infix++;
            rpn[r++] = ' ';
        }
        else if (*infix == '(')
        {
            op_stack[++top] = *infix++;
        }
        else if (*infix == ')')
        {
            while (top >= 0 && op_stack[top] != '(')
            {
                rpn[r++] = op_stack[top--];
                rpn[r++] = ' ';
            }
            if (top >= 0 && op_stack[top] == '(')
                top--;
            infix++;
        }
        else if (*infix == '+' || *infix == '-' || *infix == '*' || *infix == '/')
        {
            while (top >= 0 && get_precedence(op_stack[top]) >= get_precedence(*infix))
            {
                rpn[r++] = op_stack[top--];
                rpn[r++] = ' ';
            }
            op_stack[++top] = *infix++;
        }
        else
        {
            infix++;
        }
    }

    while (top >= 0)
    {
        rpn[r++] = op_stack[top--];
        rpn[r++] = ' ';
    }
    if (r > 0 && rpn[r - 1] == ' ')
        r--;
    return rpn;
}