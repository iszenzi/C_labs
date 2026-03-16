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
    char *buffer = (char *)malloc(capacity);
    if (!buffer)
        return NULL;

    int ch;
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
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }

    int n;
    if (fscanf(file, "%d", &n) != 1)
    {
        printf("Error: Cannot read size\n");
        fclose(file);
        return 0;
    }

    int c;
    while ((c = fgetc(file)) != '\n' && c != EOF)
        ;

    if (n > limit)
        n = limit;

    int count = 0;
    for (int i = 0; i < n; ++i)
    {
        if (fscanf(file, "%lf", &table[i].key) != 1)
        {
            break;
        }

        int c = fgetc(file);
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
