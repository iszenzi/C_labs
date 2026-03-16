#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

void free_table(MyStruct *table, int size)
{
    if (!table)
    {
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        free(table[i].value);
    }
    free(table);
}

char *read_line(FILE *file)
{
    size_t capacity = 16;
    size_t length = 0;
    int ch;
    char *buffer = (char *)malloc(capacity);
    if (!buffer)
        return NULL;

    while ((ch = fgetc(file)) != '\n' && ch != '\r' && ch != EOF)
    {
        buffer[length++] = (char)ch;
        if (length == capacity - 1)
        {
            capacity *= 2;
            char *new_buffer = (char *)realloc(buffer, capacity);
            if (!new_buffer)
            {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
    }

    if (length == 0 && ch == EOF)
    {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;
}

int read_table(MyStruct *table, int limit, const char *filename)
{
    int n;
    int c;
    int count;
    int i;
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }

    if (fscanf(file, "%d", &n) != 1)
    {
        printf("Error: Cannot read size\n");
        fclose(file);
        return 0;
    }

    while ((c = fgetc(file)) != '\n' && c != EOF)
        ;

    if (n > limit)
        n = limit;

    count = 0;
    for (i = 0; i < n; ++i)
    {
        if (fscanf(file, "%lf", &table[i].key) != 1)
        {
            break;
        }

        c = fgetc(file);
        while (c == ' ' || c == '\t')
        {
            c = fgetc(file);
        }
        if (c != EOF && c != '\n')
        {
            ungetc(c, file);
        }

        table[i].value = read_line(file);
        if (!table[i].value)
        {
            table[i].value = (char *)malloc(1);
            if (table[i].value)
                table[i].value[0] = '\0';
        }
        count++;
    }

    fclose(file);
    return count;
}

void swap(MyStruct *a, MyStruct *b)
{
    MyStruct temp = *a;
    *a = *b;
    *b = temp;
}

void shell_sort(MyStruct *table, int size)
{
    int gap, i, j;
    MyStruct current;

    for (gap = size / 2; gap > 0; gap /= 2)
    {
        for (i = gap; i < size; i++)
        {
            current = table[i];
            j = i;
            while (j >= gap && table[j - gap].key > current.key)
            {
                table[j] = table[j - gap];
                j -= gap;
            }
            table[j] = current;
        }
    }
}

int binary_search(const MyStruct *table, int size, double key)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        double mid_key = table[mid].key;

        if (mid_key == key)
        {
            return mid;
        }
        if (mid_key < key)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return -1;
}

void print_table(const MyStruct *table, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d %.3f %s\n", i, table[i].key, table[i].value ? table[i].value : "");
    }
}