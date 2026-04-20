#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

void print_menu()
{
    printf("Menu:\n");
    printf("1. Insert node\n");
    printf("2. Delete node\n");
    printf("3. Print tree\n");
    printf("4. Search node\n");
    printf("5. Process from file\n");
    printf("6. Exit\n");
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void process_file(BTreeNode **root_ptr)
{
    char in_filename[256];
    char out_filename[256];

    printf("Enter input file: ");
    if (scanf("%255s", in_filename) != 1)
        return;
    printf("Enter output file: ");
    if (scanf("%255s", out_filename) != 1)
        return;
    clear_buffer();

    FILE *in = fopen(in_filename, "r");
    if (!in)
    {
        printf("ERROR: Cannot open input file\n");
        return;
    }

    FILE *out = fopen(out_filename, "w");
    if (!out)
    {
        printf("ERROR: Cannot open output file\n");
        fclose(in);
        return;
    }

    int op;
    char key[MAX_KEY_LEN + 1];
    double value;

    while (fscanf(in, "%d", &op) == 1)
    {
        switch (op)
        {
        case 1:
            if (fscanf(in, "%6s %lf", key, &value) == 2)
            {
                fprintf(out, "1 %s %g\n", key, value);
                *root_ptr = btree_insert(*root_ptr, key, value);
                fprintf(out, "Result: Inserted\n");
            }
            break;
        case 2:
            if (fscanf(in, "%6s", key) == 1)
            {
                fprintf(out, "2 %s\n", key);
                *root_ptr = btree_delete(*root_ptr, key);
                fprintf(out, "Result: Deleted (if existed)\n");
            }
            break;
        case 3:
            fprintf(out, "3\n");
            fprintf(out, "Result: Printed to console\n");
            printf("\n--- Tree Print (from file command) ---\n");
            btree_print(*root_ptr);
            printf("--------------------------------------\n");
            break;
        case 4:
            if (fscanf(in, "%6s", key) == 1)
            {
                fprintf(out, "4 %s\n", key);
                Item *item = btree_search(*root_ptr, key);
                if (item)
                    fprintf(out, "Result: Found %.2lf\n", item->value);
                else
                    fprintf(out, "Result: Not found\n");
            }
            break;
        default:
            fprintf(out, "%d\nResult: Unknown operation\n", op);
            break;
        }
    }

    fclose(in);
    fclose(out);
    printf("File processed successfully.\n");
}

int main()
{
    BTreeNode *root = NULL;
    int command;
    char key[MAX_KEY_LEN + 1];
    double value;

    while (1)
    {
        print_menu();
        printf("Enter command ");
        if (scanf("%d", &command) != 1)
        {
            printf("Invalid input\n");
            break;
        }
        clear_buffer();

        switch (command)
        {
        case 1:
            printf("Enter key (string up to %d chars): ", MAX_KEY_LEN);
            if (scanf("%6s", key) == 1)
            {
                printf("Enter value (double): ");
                if (scanf("%lf", &value) == 1)
                {
                    root = btree_insert(root, key, value);
                    printf("Inserted successfully.\n");
                }
            }
            clear_buffer();
            break;
        case 2:
            printf("Enter key to delete: ");
            if (scanf("%6s", key) == 1)
            {
                root = btree_delete(root, key);
                printf("Deleted (if existed).\n");
            }
            clear_buffer();
            break;
        case 3:
            printf("Tree:\n");
            btree_print(root);
            break;
        case 4:
            printf("Enter key to search: ");
            if (scanf("%6s", key) == 1)
            {
                Item *item = btree_search(root, key);
                if (item)
                {
                    printf("Found: %s -> %.2lf\n", item->key, item->value);
                }
                else
                {
                    printf("Not found.\n");
                }
            }
            clear_buffer();
            break;
        case 5:
            process_file(&root);
            break;
        case 6:
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