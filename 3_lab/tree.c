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

static expr_node *copy_tree(expr_node *root)
{
    if (!root)
        return NULL;

    expr_node *new_node = (expr_node *)malloc(sizeof(expr_node));
    if (!new_node)
        return NULL;

    new_node->type = root->type;
    new_node->data = root->data;
    new_node->left = copy_tree(root->left);
    new_node->right = copy_tree(root->right);

    return new_node;
}

expr_node *reduce_multiplication(expr_node *root)
{
    if (!root)
        return NULL;

    root->left = reduce_multiplication(root->left);
    root->right = reduce_multiplication(root->right);

    if (root->type == NODE_OP && root->data.op == '*')
    {
        expr_node *expr = NULL;
        int n = 0;

        if (root->right && root->right->type == NODE_NUM && root->right->data.num > 0)
        {
            n = root->right->data.num;
            expr = root->left;
        }
        else if (root->left && root->left->type == NODE_NUM && root->left->data.num > 0)
        {
            n = root->left->data.num;
            expr = root->right;
        }

        if (expr && n > 0)
        {
            if (n == 1)
            {
                expr_node *new_root = copy_tree(expr);
                free_tree(root);
                return new_root;
            }

            expr_node *sum = create_op_node('+', copy_tree(expr), copy_tree(expr));
            for (int i = 2; i < n; i++)
            {
                sum = create_op_node('+', sum, copy_tree(expr));
            }

            free_tree(root);
            return sum;
        }
    }

    return root;
}

void print_infix(expr_node *root)
{
    if (!root)
        return;

    if (root->type == NODE_OP)
    {
        if (root->left == NULL)
        {
            if (root->data.op == '-')
                printf("-");
            else if (root->data.op == '+')
                printf("+");
            print_infix(root->right);
        }
        else
        {
            printf("(");
            print_infix(root->left);
            printf(" %c ", root->data.op);
            print_infix(root->right);
            printf(")");
        }
    }
    else if (root->type == NODE_VAR)
    {
        printf("%c", root->data.var);
    }
    else if (root->type == NODE_NUM)
    {
        printf("%d", root->data.num);
    }
}

void print_tree_visual(expr_node *root, int depth)
{
    if (!root)
        return;

    print_tree_visual(root->right, depth + 1);

    for (int i = 0; i < depth; i++)
        printf("    ");

    if (root->type == NODE_OP)
    {
        if (root->left == NULL)
            printf("u%c\n", root->data.op);
        else
            printf("%c\n", root->data.op);
    }
    else if (root->type == NODE_VAR)
        printf("%c\n", root->data.var);
    else if (root->type == NODE_NUM)
        printf("%d\n", root->data.num);

    print_tree_visual(root->left, depth + 1);
}