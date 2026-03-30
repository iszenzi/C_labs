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
    case '~':
    case '#':
        return 3;
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

    bool expect_operand = true;

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
            expect_operand = false;
        }
        else if (isalpha(*infix))
        {
            rpn[r++] = *infix++;
            rpn[r++] = ' ';
            expect_operand = false;
        }
        else if (*infix == '(')
        {
            op_stack[++top] = *infix++;
            expect_operand = true;
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
            expect_operand = false;
        }
        else if (*infix == '+' || *infix == '-' || *infix == '*' || *infix == '/')
        {
            char current_op = *infix;

            if (expect_operand && (current_op == '+' || current_op == '-'))
            {
                current_op = (current_op == '-') ? '~' : '#';
            }

            while (top >= 0 && get_precedence(op_stack[top]) >= get_precedence(current_op))
            {
                if ((current_op == '~' || current_op == '#') && get_precedence(op_stack[top]) == get_precedence(current_op))
                {
                    break;
                }
                rpn[r++] = op_stack[top--];
                rpn[r++] = ' ';
            }
            op_stack[++top] = current_op;
            infix++;
            expect_operand = true;
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
    rpn[r] = '\0';

    return rpn;
}

expr_node *parse_expression(const char *str)
{
    char *rpn = infix_to_rpn(str);
    if (!rpn || strlen(rpn) == 0)
    {
        free(rpn);
        return NULL;
    }

    expr_node *stack[512];
    int top = -1;

    const char *p = rpn;
    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (isdigit(*p))
        {
            int num = 0;
            while (isdigit(*p))
            {
                num = num * 10 + (*p - '0');
                p++;
            }
            stack[++top] = create_num_node(num);
        }
        else if (isalpha(*p))
        {
            stack[++top] = create_var_node(*p);
            p++;
        }
        else if (*p == '+' || *p == '-' || *p == '*' || *p == '/')
        {
            if (top >= 1)
            {
                expr_node *right = stack[top--];
                expr_node *left = stack[top--];
                stack[++top] = create_op_node(*p, left, right);
            }
            p++;
        }
        else if (*p == '~' || *p == '#')
        {
            if (top >= 0)
            {
                expr_node *operand = stack[top--];
                char op_char = (*p == '~') ? '-' : '+';
                stack[++top] = create_op_node(op_char, NULL, operand);
            }
            p++;
        }
        else
        {
            p++;
        }
    }

    expr_node *root = (top >= 0) ? stack[top--] : NULL;
    free(rpn);
    return root;
}