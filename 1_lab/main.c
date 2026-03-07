#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
// спросить про импортирование файлов
// запуск тестов это всегда отдельный таргет, который не зависит от таргета для запуска программы
void print_menu()
{
    printf("Menu:\n");
    printf("1. Insert node\n");
    printf("2. Print tree\n");
    printf("3. Delete node\n");
    printf("4. Check range\n");
    printf("5. Exit\n");
}

int main()
{
    node *root = NULL;
    int data, command, min, max;

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
            printf("Enter data ");
            scanf("%d", &data);
            root = delete_node(root, data);
            break;
        case 4:
            printf("Enter lower border ");
            scanf("%d", &min);
            printf("Enter upper border ");
            scanf("%d", &max);
            if (check_range(root, min, max))
            {
                printf("ALL leaves in range [%d, %d]\n", min, max);
            }
            else
            {
                printf("NOT ALL leaves in range [%d, %d]\n", min, max);
            }
            break;
        case 5:
            tree_free(root);
            exit(0);
        default:
            printf("Invalid command\n");
            break;
        }
    }
}