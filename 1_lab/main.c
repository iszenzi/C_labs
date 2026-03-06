#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "tree.c"

void print_menu()
{
    printf("Menu:\n");
    printf("1. Insert node\n");
    printf("2. Print tree\n");
    printf("3. Exit\n");
}

int main()
{
    node *root = NULL;
    int data, command;

    while (1)
    {
        print_menu();
        printf("Enter command ");
        scanf("%d", &command);

        switch (command)
        {
        case 1:
            printf("Enter data ");
            scanf("%d", &data);
            root = insert_node(root, data);
            break;
        case 2:
            tree_print(root, 0);
            break;
        case 3:
            tree_free(root);
            exit(0);
        default:
            break;
        }
    }
}