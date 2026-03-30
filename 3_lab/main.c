#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void print_menu()
{
    printf("Menu:\n");
    printf("1. Enter expression\n");
    printf("2. Print tree\n");
    printf("3. Reduce tree\n");
    printf("4. Exit\n");
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main()
{
    expr_node *root = NULL;
    char buffer[512] = {0};
    int command;

    while (1)
    {
        print_menu();
        printf("Enter command ");
        if (scanf("%d", &command) != 1)
        {
            printf("Invalid input\n");
            return 1;
        }
        clear_buffer();

        switch (command)
        {
        case 1:
        {
            printf("Enter expression ");
            if (fgets(buffer, sizeof(buffer), stdin))
            {
                buffer[strcspn(buffer, "\n")] = '\0';

                if (root)
                {
                    free_tree(root);
                    root = NULL;
                }

                char *rpn = infix_to_rpn(buffer);
                if (rpn)
                {
                    printf("RPN: %s\n", rpn);
                    free(rpn);
                }

                root = parse_expression(buffer);
                if (!root)
                {
                    printf("ERROR: Parse failed\n");
                }
            }
            break;
        }
        case 2:
            if (!root)
            {
                printf("ERROR: Enter expression first\n");
            }
            else
            {
                printf("Tree:\n");
                print_tree_visual(root, 0);
                printf("Infix: ");
                print_infix(root);
                printf("\n");

                if (!has_variables(root))
                {
                    double result = evaluate_tree(root);
                    printf("Result: %g\n", result);
                }
                else
                {
                    printf("Result: Contains variables, cannot calculate.\n");
                }
            }
            break;
        case 3:
            if (!root)
            {
                printf("ERROR: Enter expression first\n");
            }
            else
            {
                root = reduce_multiplication(root);
                printf("Tree after reduce:\n");
                print_tree_visual(root, 0);
                printf("Infix: ");
                print_infix(root);
                printf("\n");

                if (!has_variables(root))
                {
                    double result = evaluate_tree(root);
                    printf("Result: %g\n", result);
                }
                else
                {
                    printf("Result: Contains variables, cannot calculate.\n");
                }
            }
            break;
        case 4:
            if (root)
            {
                free_tree(root);
            }
            exit(0);
        default:
            printf("Invalid command\n");
            break;
        }
    }
    return 0;
}