#include <stdio.h>
#include <stdlib.h>
#include "table.h"

#define MAX_RECORDS 100

void print_menu(const char *current_file)
{
    printf("Menu:\n");
    if (current_file && current_file[0] != '\0')
        printf("Current file: %s\n", current_file);
    else
        printf("Current file: (none)\n");
    printf("1. Load table from file\n");
    printf("2. Print table\n");
    printf("3. Shell sort\n");
    printf("4. Binary search\n");
    printf("5. Exit\n");
}

int main()
{
    MyStruct *table = NULL;
    int size = 0;
    int command;

    char *current_file = NULL;

    while (1)
    {
        print_menu(current_file);
        printf("Enter command ");
        if (scanf("%d", &command) != 1)
        {
            printf("Invalid input\n");
            return 1;
        }

        switch (command)
        {
        case 1:
        {
            char filename[256];
            printf("Enter filename ");
            scanf("%255s", filename);

            if (table)
            {
                free_table(table, size);
                table = NULL;
                size = 0;
            }

            table = (MyStruct *)malloc(sizeof(MyStruct) * MAX_RECORDS);
            if (!table)
            {
                printf("ERROR: Memory allocation\n");
                return 1;
            }

            size = read_table(table, MAX_RECORDS, filename);
            if (size == 0)
            {
                printf("ERROR: Table is empty or file error\n");
            }
            else if (size < 15)
            {
                printf("ERROR: Less than 15 elements (%d)\n", size);
            }
            else
            {
                if (current_file)
                    free(current_file);
                current_file = strdup(filename);
            }
            break;
        }
        case 2:
            if (!table)
            {
                printf("ERROR: Load table first\n");
            }
            else
            {
                print_table(table, size);
            }
            break;
        case 3:
            if (!table)
            {
                printf("ERROR: Load table first\n");
            }
            else
            {
                printf("Before sort:\n");
                print_table(table, size);
                shell_sort(table, size);
                printf("After sort:\n");
                print_table(table, size);
            }
            break;
        case 4:
            if (!table)
            {
                printf("ERROR: Load table first\n");
            }
            else
            {
                double key;
                int index;
                printf("Enter key ");
                scanf("%lf", &key);

                index = binary_search(table, size, key);
                if (index >= 0)
                {
                    printf("Found: %g %s\n", table[index].key,
                           table[index].value ? table[index].value : "");
                }
                else
                {
                    printf("Not found\n");
                }
            }
            break;
        case 5:
            if (table)
            {
                free_table(table, size);
            }
            if (current_file)
                free(current_file);
            exit(0);
        default:
            printf("Invalid command\n");
            break;
        }
    }
}
